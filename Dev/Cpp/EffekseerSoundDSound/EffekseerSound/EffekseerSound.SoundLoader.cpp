
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include <string.h>
#include "EffekseerSound.SoundImplemented.h"
#include "EffekseerSound.SoundLoader.h"

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerSound
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
SoundLoader::SoundLoader(SoundImplemented* sound)
	: m_sound(sound)
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
SoundLoader::~SoundLoader()
{
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void* SoundLoader::Load( const EFK_CHAR* path )
{
	HRESULT hr;

	FILE* fp = NULL;
	errno_t err = _wfopen_s(&fp, (const wchar_t*)path, L"rb");
	if (fp == NULL) {
		return NULL;
	}

	uint32_t chunkIdent, chunkSize;
	// RIFF�`�����N���`�F�b�N
	fread(&chunkIdent, 1, 4, fp);
	fread(&chunkSize, 1, 4, fp);
	if (memcmp(&chunkIdent, "RIFF", 4) != 0) {
		return NULL;
	}

	// WAVE�V���{�����`�F�b�N
	fread(&chunkIdent, 1, 4, fp);
	if (memcmp(&chunkIdent, "WAVE", 4) != 0) {
		return NULL;
	}
	
	WAVEFORMATEX wavefmt = {0};
	for (;;) {
		fread(&chunkIdent, 1, 4, fp);
		fread(&chunkSize, 1, 4, fp);

		if (memcmp(&chunkIdent, "fmt ", 4) == 0) {
			// �t�H�[�}�b�g�`�����N
			uint32_t size = min(chunkSize, sizeof(wavefmt));
			fread(&wavefmt, 1, size, fp);
			if (size < chunkSize) {
				fseek(fp, chunkSize - size, SEEK_CUR);
			}
		} else if (memcmp(&chunkIdent, "data", 4) == 0) {
			// �f�[�^�`�����N
			break;
		} else {
			// �s���ȃ`�����N�̓X�L�b�v
			fseek(fp, chunkSize, SEEK_CUR);
		}
	}
	
	// �t�H�[�}�b�g�`�F�b�N
	if (wavefmt.wFormatTag != WAVE_FORMAT_PCM || wavefmt.nChannels > 2) {
		return NULL;
	}

	// DirectSound�o�b�t�@���쐬
	DSBUFFERDESC dsdesc;
    ZeroMemory(&dsdesc,sizeof(DSBUFFERDESC));
    dsdesc.dwSize = sizeof(DSBUFFERDESC);
	dsdesc.dwFlags = DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLVOLUME | 
		DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN;
    dsdesc.dwBufferBytes = chunkSize;
    dsdesc.lpwfxFormat = &wavefmt;
    dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;

	IDirectSoundBuffer* dsbufTmp = 0;
	IDirectSoundBuffer8* dsbuf = NULL;
	hr = m_sound->GetDevice()->CreateSoundBuffer(&dsdesc, &dsbufTmp, NULL);
	if (hr == DS_OK) {
		hr = dsbufTmp->QueryInterface(IID_IDirectSoundBuffer8, (void**)&dsbuf);
		dsbufTmp->Release();
	}
	if (hr != DS_OK) {
		fclose(fp);
        return FALSE;
    }

    // �o�b�t�@�����b�N���ă��[�h
    LPVOID bufptr;
    DWORD bufsize;
	hr = dsbuf->Lock(0, 0, &bufptr, &bufsize, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	if (hr == DS_OK) {
		fread(bufptr, bufsize, 1, fp);
		hr = dsbuf->Unlock(bufptr, bufsize, NULL, 0);
	}
	fclose(fp);

	SoundData* soundData = new SoundData;
	soundData->channels = wavefmt.nChannels;
	soundData->sampleRate = wavefmt.nSamplesPerSec;
	soundData->buffer = dsbuf;

	return soundData;
}
	
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void SoundLoader::Unload( void* data )
{
	SoundData* soundData = (SoundData*)data;
	if (soundData == NULL) {
		return;
	}

	m_sound->StopData(soundData);
	soundData->buffer->Release();
	delete soundData;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
