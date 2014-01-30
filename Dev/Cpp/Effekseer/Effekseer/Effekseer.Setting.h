
#ifndef	__EFFEKSEER_LOADER_H__
#define	__EFFEKSEER_LOADER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "Effekseer.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	�ݒ�N���X
	@note
	EffectLoader���A�S�Ẵ����_���[�A���[�_�[��ݒ肷�邱�Ƃ��ł��AManager�̑���ɃG�t�F�N�g�ǂݍ��ݎ��Ɏg�p����B
*/
class Setting
{
private:
	EffectLoader*	m_effectLoader;
	TextureLoader*	m_textureLoader;
	SoundLoader*	m_soundLoader;
	ModelLoader*	m_modelLoader;

public:
	/**
		@brief	�R���X�g���N�^
	*/
	Setting();

	/**
		@brief	�f�X�g���N�^
	*/
	virtual ~Setting();

	/**
		@brief	�G�t�F�N�g���[�_�[���擾����B
		@return	�G�t�F�N�g���[�_�[
	*/
	EffectLoader* GetEffectLoader();

	/**
		@brief	�G�t�F�N�g���[�_�[��ݒ肷��B
		@param	loader	[in]		���[�_�[
	*/
	void SetEffectLoader(EffectLoader* loader);

	/**
		@brief	�e�N�X�`�����[�_�[���擾����B
		@return	�e�N�X�`�����[�_�[
	*/
	TextureLoader* GetTextureLoader();

	/**
		@brief	�e�N�X�`�����[�_�[��ݒ肷��B
		@param	loader	[in]		���[�_�[
	*/
	void SetTextureLoader(TextureLoader* loader);

	/**
		@brief	���f�����[�_�[���擾����B
		@return	���f�����[�_�[
	*/
	ModelLoader* GetModelLoader();

	/**
		@brief	���f�����[�_�[��ݒ肷��B
		@param	loader	[in]		���[�_�[
	*/
	void SetModelLoader(ModelLoader* loader);

	/**
		@brief	�T�E���h���[�_�[���擾����B
		@return	�T�E���h���[�_�[
	*/
	SoundLoader* GetSoundLoader();

	/**
		@brief	�T�E���h���[�_�[��ݒ肷��B
		@param	loader	[in]		���[�_�[
	*/
	void SetSoundLoader(SoundLoader* loader);
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_LOADER_H__
