
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "DxLib.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib" )
#pragma comment(lib, "d3dx9.lib" )

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <Effekseer.h>
#include <EffekseerRendererDX9.h>

#if _DEBUG
#pragma comment(lib, "Effekseer.Debug.lib" )
#pragma comment(lib, "EffekseerRendererDX9.Debug.lib" )
#else
#pragma comment(lib, "Effekseer.Release.lib" )
#pragma comment(lib, "EffekseerRendererDX9.Release.lib" )
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
static int g_window_width = 640;
static int g_window_height = 480;
static ::Effekseer::Manager*			g_manager = NULL;
static ::EffekseerRenderer::Renderer*	g_renderer = NULL;
static ::Effekseer::Effect*				g_effect = NULL;
static ::Effekseer::Handle				g_handle = -1;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void DeviceLostFunction( void *Data )
{
	printf("Start LostFunction\n");
	// �f�o�C�X���X�g�������������ɌĂԁB
	g_renderer->OnLostDevice();

	// �ǂݍ��񂾃G�t�F�N�g�̃��\�[�X�͑S�Ĕj������B
	g_effect->UnloadResources();

	// DX���C�u�����͓����Ńf�o�C�X���̂��������Ă���̂�NULL��ݒ肷��B
	g_renderer->ChangeDevice( NULL );

	printf("End LostFunction\n");
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void DeviceRestoreFunction( void *Data )
{
	printf("Start RestoreFunction\n");

	// DX���C�u�����͉񕜎��ɓ����Ńf�o�C�X���Đ�������̂ŐV�����ݒ肷��B
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();
	g_renderer->ChangeDevice( device );

	// �G�t�F�N�g�̃��\�[�X���ēǂݍ��݂���B
	g_effect->ReloadResources();

	// �f�o�C�X�����A����Ƃ��ɌĂ�
	g_renderer->OnResetDevice();
	printf("End RestoreFunction\n");
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int main()
{
	// �E�B���h�E���[�h�ɕύX
	ChangeWindowMode( true );					

	// ������
    if( DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) != 0 ) return -1;
	
	// �`��p�C���X�^���X�̐���
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();
	g_renderer = ::EffekseerRenderer::Renderer::Create( device, 2000 );

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h��
	SetChangeScreenModeGraphicsSystemResetFlag( FALSE );

	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	g_manager = ::Effekseer::Manager::Create( 2000 );

	// �`��p�C���X�^���X����`��@�\��ݒ�
	g_manager->SetSpriteRenderer( g_renderer->CreateSpriteRenderer() );
	g_manager->SetRibbonRenderer( g_renderer->CreateRibbonRenderer() );
	g_manager->SetRingRenderer( g_renderer->CreateRingRenderer() );
	g_manager->SetModelRenderer( g_renderer->CreateModelRenderer() );
	g_manager->SetTrackRenderer( g_renderer->CreateTrackRenderer() );

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	g_manager->SetTextureLoader( g_renderer->CreateTextureLoader() );
	g_manager->SetModelLoader( g_renderer->CreateModelLoader() );

	// �f�o�C�X���X�g���̃R�[���o�b�N�ݒ�(�t���X�N���[���E�C���h�E�؂�ւ��̂��߂ɕK�v)
	SetGraphicsDeviceLostCallbackFunction( DeviceLostFunction, NULL );
	SetGraphicsDeviceRestoreCallbackFunction( DeviceRestoreFunction, NULL );

	// ���e�s���ݒ�
	g_renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovRH( 90.0f / 180.0f * 3.14f, (float)g_window_width / (float)g_window_height, 1.0f, 50.0f ) );
	
	// �J�����s���ݒ�
	g_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH( ::Effekseer::Vector3D( 10.0f, 5.0f, 20.0f ), ::Effekseer::Vector3D( 0.0f, 0.0f, 0.0f ), ::Effekseer::Vector3D( 0.0f, 1.0f, 0.0f ) ) );
	
	// �G�t�F�N�g�̓Ǎ�
	g_effect = Effekseer::Effect::Create( g_manager, (const EFK_CHAR*)L"test.efk" );

	// ���ł���������摜�Ǎ�
	int grHandle = LoadGraph( L"Texture/Particle01.png" );

	// ���Ԃ̐ݒ�
	int time = 0;

	bool isFullScreen = false;

	while( !ProcessMessage() && !ClearDrawScreen() && !CheckHitKey(KEY_INPUT_ESCAPE) )
	{
		// ����I�ɃG�t�F�N�g���Đ�
		if( time % 60 == 0 )
		{
			// �G�t�F�N�g�̍Đ�
			g_handle = g_manager->Play( g_effect, 0, 0, 0 );
		}

		// ���ł������̂�TransFrag��L���ɂ��ĉ摜��`�悷��B
		// �������ĕ`�悵����łȂ��ƁAEffekseer�͕`��ł��Ȃ��B
		DrawGraph( 0, 0, grHandle, TRUE );

		// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o��
		RenderVertex();
		
		// �G�t�F�N�g�̈ړ��������s��
		g_manager->AddLocation( g_handle, ::Effekseer::Vector3D( 0.2f, 0.0f, 0.0f ) );

		// �G�t�F�N�g�̍X�V�������s��
		g_manager->Update();
			
		// �G�t�F�N�g�̕`��J�n�������s���B
		g_renderer->BeginRendering();

		// �G�t�F�N�g�̕`����s���B
		g_manager->Draw();

		// �G�t�F�N�g�̕`��I���������s���B
		g_renderer->EndRendering();
		
		// DX���C�u�����̐ݒ��߂��B
		RefreshDxLibDirect3DSetting();

		// �X�N���[������ւ�
		ScreenFlip();

		// ���Ԍo��
		time++;

		// �t���X�N���[���؂�ւ�
		if( CheckHitKey(KEY_INPUT_F1) && !isFullScreen )
		{
			ChangeWindowMode( FALSE );
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = true;
		}
		if( CheckHitKey(KEY_INPUT_F2) && isFullScreen )
		{
			ChangeWindowMode( TRUE );
			SetDrawScreen(DX_SCREEN_BACK);
			isFullScreen = false;
		}
	}

	// �G�t�F�N�g�̔j��
	ES_SAFE_RELEASE( g_effect );

	// �G�t�F�N�g�Ǘ��p�C���X�^���X��j��
	g_manager->Destroy();

	// �`��p�C���X�^���X��j��
	g_renderer->Destory();

	DxLib_End();

	return 0;
}
