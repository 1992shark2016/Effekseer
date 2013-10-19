
#ifndef	__EFFEKSEERTOOL_RENDERER_H__
#define	__EFFEKSEERTOOL_RENDERER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerTool.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerTool
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class Renderer
{
private:
	HWND				m_handle;
	int32_t				m_width;
	int32_t				m_height;
	LPDIRECT3D9			m_d3d;
	LPDIRECT3DDEVICE9	m_d3d_device;

	int32_t				m_squareMaxCount;

	eProjectionType		m_projection;

	::EffekseerRenderer::RendererImplemented*	m_renderer;

	::EffekseerRenderer::Grid*	m_grid;
	::EffekseerRenderer::Guide*	m_guide;
	::EffekseerRenderer::Background*	m_background;

	bool		m_recording;

	IDirect3DSurface9*	m_recordingTarget;
	IDirect3DTexture9*	m_recordingTargetTexture;
	IDirect3DSurface9*	m_recordingDepth;

	IDirect3DSurface9*	m_recordingTempTarget;
	IDirect3DSurface9*	m_recordingTempDepth;

	IDirect3DTexture9*	m_backGroundTexture;
public:
	/**
		@brief	�R���X�g���N�^
	*/
	Renderer( int32_t squareMaxCount );

	/**
		@brief	�f�X�g���N�^
	*/
	~Renderer();

	/**
		@brief	���������s���B
	*/
	bool Initialize( HWND handle, int width, int height );

	/**
		@brief	�f�o�C�X���擾����B
	*/
	LPDIRECT3DDEVICE9 GetDevice();

	/**
		@brief	��ʂɕ\������B
	*/
	bool Present();

	/**
		@brief	�f�o�C�X�̃��Z�b�g
	*/
	void ResetDevice();

	/**
		@brief	�ˉe�擾
	*/
	eProjectionType GetProjectionType();

	/**
		@brief	�ˉe�ݒ�
	*/
	void SetProjectionType( eProjectionType type );

	/**
		@brief	��ʃT�C�Y�ύX
	*/
	bool Resize( int width, int height );

	void RecalcProjection();

	::EffekseerRenderer::Renderer*	GetRenderer() { return m_renderer; };

	/**
		@brief	�ˉe�s��ݒ�
	*/
	void SetPerspectiveFov( int width, int height );

	/**
		@brief	�ˉe�s��ݒ�
	*/
	void SetOrthographic( int width, int height );

	/**
		@brief	Orthographic�\���̊g�嗦
	*/
	float	RateOfMagnification;

	/**
		@brief	�K�C�h�̏c��
	*/
	int32_t GuideWidth;

	/**
		@brief	�K�C�h�̉���
	*/
	int32_t	GuideHeight;

	/**
		@brief	�K�C�h��`�悷�邩�ǂ���
	*/
	bool RendersGuide;

	/**
		@brief	�O���b�h��\�����邩?
	*/
	bool IsGridShown;

	/**
		@brief	XY�O���b�h��\�����邩?
	*/
	bool IsGridXYShown;

	/**
		@brief	XZ�O���b�h��\�����邩?
	*/
	bool IsGridXZShown;

	/**
		@brief	YZ�O���b�h��\�����邩?
	*/
	bool IsGridYZShown;

	/**
		@brief	�E��n��?
	*/
	bool IsRightHand;

	/**
		@brief	�O���b�h�̒���
	*/
	float GridLength;

	/**
		@brief	�w�i�F
	*/
	Effekseer::Color GridColor;

	/**
		@brief	�w�i�F
	*/
	Effekseer::Color BackgroundColor;

	/**
		@brief	�w�i����������?
	*/
	bool IsBackgroundTranslucent;

	/**
		@brief	�`��J�n
	*/
	bool BeginRendering();

	/**
		@brief	�`��I��
	*/
	bool EndRendering();

	/**
		@brief	�^��J�n
	*/
	bool BeginRecord( int32_t width, int32_t height );

	/**
		@brief	�^��ʒu�ύX
	*/
	void SetRecordRect( int32_t x, int32_t y );

	/**
		@brief	�^��I��
	*/
	void EndRecord( const wchar_t* outputPath );

	/**
		@brief	�w�i�̓ǂݍ���
	*/
	void LoadBackgroundImage( void* data, int32_t size );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERTOOL_RENDERER_H__