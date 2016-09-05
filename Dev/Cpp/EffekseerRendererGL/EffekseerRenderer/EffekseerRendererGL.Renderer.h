
#ifndef	__EFFEKSEERRENDERER_GL_RENDERER_H__
#define	__EFFEKSEERRENDERER_GL_RENDERER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerRendererGL.Base.h"
#include "../../EffekseerRendererCommon/EffekseerRenderer.Renderer.h"

//----------------------------------------------------------------------------------
// Lib
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	�`��N���X
*/
class Renderer
	: public ::EffekseerRenderer::Renderer
{
protected:
	Renderer() {}
	virtual ~Renderer() {}

public:
	/**
		@brief	�C���X�^���X�𐶐�����B
		@param	squareMaxCount		�ő�`��X�v���C�g��
		@param	OpenGLDeviceType	�f�o�C�X�̎��
		@return	�C���X�^���X
	*/
	static Renderer* Create(int32_t squareMaxCount, OpenGLDeviceType deviceType = OpenGLDeviceType::OpenGL2);

	/**
		@brief	�ő�`��X�v���C�g�����擾����B
	*/
	virtual int32_t GetSquareMaxCount() const = 0;

	/**
		@brief	�ő�`��X�v���C�g����ݒ肷��B
		@note
		�`�悵�Ă��鎞�͎g�p�ł��Ȃ��B
	*/
	virtual void SetSquareMaxCount(int32_t count) = 0;

	/**
	@brief	�w�i���擾����B
	*/
	virtual GLuint GetBackground() = 0;

	/**
	@brief	�w�i��ݒ肷��B
	*/
	virtual void SetBackground(GLuint background) = 0;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	���f��
*/
class Model
{
private:

public:
	GLuint		VertexBuffer;
	GLuint		IndexBuffer;
	int32_t		VertexCount;
	int32_t		IndexCount;
	int32_t		ModelCount;

	Model( ::Effekseer::Model::Vertex vertexData[], int32_t vertexCount, 
		::Effekseer::Model::Face faceData[], int32_t faceCount );
	~Model();
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_RENDERER_H__