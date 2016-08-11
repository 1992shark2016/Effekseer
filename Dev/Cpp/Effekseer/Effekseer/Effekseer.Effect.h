
#ifndef	__EFFEKSEER_EFFECT_H__
#define	__EFFEKSEER_EFFECT_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "Effekseer.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	�G�t�F�N�g�p�����[�^�[�N���X
	@note
	�G�t�F�N�g�ɐݒ肳�ꂽ�p�����[�^�[�B
*/
class Effect
	: public IReference
{
protected:
	Effect() {}
    ~Effect() {}

public:

	/**
		@brief	�G�t�F�N�g�𐶐�����B
		@param	manager			[in]	�Ǘ��N���X
		@param	data			[in]	�f�[�^�z��̐擪�̃|�C���^
		@param	size			[in]	�f�[�^�z��̒���
		@param	magnification	[in]	�ǂݍ��ݎ��̊g�嗦
		@param	materialPath	[in]	�f�ރ��[�h���̊�p�X
		@return	�G�t�F�N�g�B���s�����ꍇ��NULL��Ԃ��B
	*/
	static Effect* Create( Manager* manager, void* data, int32_t size, float magnification = 1.0f, const EFK_CHAR* materialPath = NULL );

	/**
		@brief	�G�t�F�N�g�𐶐�����B
		@param	manager			[in]	�Ǘ��N���X
		@param	path			[in]	�Ǎ����̃p�X
		@param	magnification	[in]	�ǂݍ��ݎ��̊g�嗦
		@param	materialPath	[in]	�f�ރ��[�h���̊�p�X
		@return	�G�t�F�N�g�B���s�����ꍇ��NULL��Ԃ��B
	*/
	static Effect* Create( Manager* manager, const EFK_CHAR* path, float magnification = 1.0f, const EFK_CHAR* materialPath = NULL );

		/**
		@brief	�G�t�F�N�g�𐶐�����B
		@param	setting			[in]	�ݒ�N���X
		@param	data			[in]	�f�[�^�z��̐擪�̃|�C���^
		@param	size			[in]	�f�[�^�z��̒���
		@param	magnification	[in]	�ǂݍ��ݎ��̊g�嗦
		@param	materialPath	[in]	�f�ރ��[�h���̊�p�X
		@return	�G�t�F�N�g�B���s�����ꍇ��NULL��Ԃ��B
	*/
	static Effect* Create( Setting*	setting, void* data, int32_t size, float magnification = 1.0f, const EFK_CHAR* materialPath = NULL );

	/**
		@brief	�G�t�F�N�g�𐶐�����B
		@param	setting			[in]	�ݒ�N���X
		@param	path			[in]	�Ǎ����̃p�X
		@param	magnification	[in]	�ǂݍ��ݎ��̊g�嗦
		@param	materialPath	[in]	�f�ރ��[�h���̊�p�X
		@return	�G�t�F�N�g�B���s�����ꍇ��NULL��Ԃ��B
	*/
	static Effect* Create( Setting*	setting, const EFK_CHAR* path, float magnification = 1.0f, const EFK_CHAR* materialPath = NULL );

	/**
	@brief	�W���̃G�t�F�N�g�Ǎ��C���X�^���X�𐶐�����B
	*/
	static ::Effekseer::EffectLoader* CreateEffectLoader(::Effekseer::FileInterface* fileInterface = NULL);

	/**
	@brief	�ݒ���擾����B
	@return	�ݒ�
	*/
	virtual Setting* GetSetting() const = 0;

	/* �g�嗦���擾����B */
	virtual float GetMaginification() const = 0;
	
	/**
		@brief	�G�t�F�N�g�f�[�^�̃o�[�W�����擾
	*/
	virtual int GetVersion() const = 0;

	/**
		@brief	�i�[����Ă���F�摜�̃|�C���^���擾����B
		@param	n	[in]	�摜�̃C���f�b�N�X
		@return	�摜�̃|�C���^
	*/
	virtual void* GetColorImage( int n ) const = 0;

	/**
	@brief	�i�[����Ă���摜�̃|�C���^�̌����擾����B
	*/
	virtual int32_t GetColorImageCount() const = 0;

	/**
	@brief	�i�[����Ă���@���摜�̃|�C���^���擾����B
	@param	n	[in]	�摜�̃C���f�b�N�X
	@return	�摜�̃|�C���^
	*/
	virtual void* GetNormalImage(int n) const = 0;

	/**
	@brief	�i�[����Ă���@���摜�̃|�C���^�̌����擾����B
	*/
	virtual int32_t GetNormalImageCount() const = 0;

	/**
	@brief	�i�[����Ă���c�݉摜�̃|�C���^���擾����B
	@param	n	[in]	�摜�̃C���f�b�N�X
	@return	�摜�̃|�C���^
	*/
	virtual void* GetDistortionImage(int n) const = 0;

	/**
	@brief	�i�[����Ă���c�݉摜�̃|�C���^�̌����擾����B
	*/
	virtual int32_t GetDistortionImageCount() const = 0;

	/**
		@brief	�i�[����Ă��鉹�g�`�̃|�C���^���擾����B
	*/
	virtual void* GetWave( int n ) const = 0;

	/**
	@brief	�i�[����Ă��鉹�g�`�̃|�C���^�̌����擾����B
	*/
	virtual int32_t GetWaveCount() const = 0;

	/**
		@brief	�i�[����Ă��郂�f���̃|�C���^���擾����B
	*/
	virtual void* GetModel( int n ) const = 0;

	/**
	@brief	�i�[����Ă��郂�f���̃|�C���^�̌����擾����B
	*/
	virtual int32_t GetModelCount() const = 0;

	/**
		@brief	�G�t�F�N�g�̃����[�h���s���B
	*/
	virtual bool Reload( void* data, int32_t size, const EFK_CHAR* materialPath = NULL ) = 0;

	/**
		@brief	�G�t�F�N�g�̃����[�h���s���B
	*/
	virtual bool Reload( const EFK_CHAR* path, const EFK_CHAR* materialPath = NULL ) = 0;

	/**
		@brief	�G�t�F�N�g�̃����[�h���s���B
		@param	managers	[in]	�}�l�[�W���[�̔z��
		@param	managersCount	[in]	�}�l�[�W���[�̌�
		@param	data	[in]	�G�t�F�N�g�̃f�[�^
		@param	size	[in]	�G�t�F�N�g�̃f�[�^�T�C�Y
		@param	materialPath	[in]	���\�[�X�̓ǂݍ��݌�
		@return	����
		@note
		Setting��p���ăG�t�F�N�g�𐶐������Ƃ��ɁAManager���w�肷�邱�ƂőΏۂ�Manager���̃G�t�F�N�g�̃����[�h���s���B
	*/
	virtual bool Reload( Manager* managers, int32_t managersCount, void* data, int32_t size, const EFK_CHAR* materialPath = NULL ) = 0;

	/**
	@brief	�G�t�F�N�g�̃����[�h���s���B
	@param	managers	[in]	�}�l�[�W���[�̔z��
	@param	managersCount	[in]	�}�l�[�W���[�̌�
	@param	path	[in]	�G�t�F�N�g�̓ǂݍ��݌�
	@param	materialPath	[in]	���\�[�X�̓ǂݍ��݌�
	@return	����
	@note
	Setting��p���ăG�t�F�N�g�𐶐������Ƃ��ɁAManager���w�肷�邱�ƂőΏۂ�Manager���̃G�t�F�N�g�̃����[�h���s���B
	*/
	virtual bool Reload( Manager* managers, int32_t managersCount,const EFK_CHAR* path, const EFK_CHAR* materialPath = NULL ) = 0;

	/**
		@brief	�摜�����\�[�X�̍ēǂݍ��݂��s���B
	*/
	virtual void ReloadResources( const EFK_CHAR* materialPath = NULL ) = 0;

	/**
		@brief	�摜�����\�[�X�̔j�����s���B
	*/
	virtual void UnloadResources() = 0;

	/**
	@brief	Root���擾����B
	*/
	virtual EffectNode* GetRoot() const = 0;
};

/**
@brief	���ʕ`��p�����[�^�[
@note
�傫���ύX�����\��������܂��B
*/
struct EffectBasicRenderParameter
{
	int32_t				ColorTextureIndex;
	AlphaBlendType		AlphaBlend;
	TextureFilterType	FilterType;
	TextureWrapType		WrapType;
	bool				ZWrite;
	bool				ZTest;
	bool				Distortion;
	float				DistortionIntensity;
};

/**
@brief	�m�[�h�C���X�^���X�����N���X
@note
�G�t�F�N�g�̃m�[�h�̎��̂𐶐�����B
*/
class EffectNode
{
public:
	EffectNode() {}
	virtual ~EffectNode(){}

	/**
	@brief	�m�[�h���������Ă���G�t�F�N�g���擾����B
	*/
	virtual Effect* GetEffect() const = 0;

	/**
	@brief	�q�̃m�[�h�̐����擾����B
	*/
	virtual int GetChildrenCount() const = 0;

	/**
	@brief	�q�̃m�[�h���擾����B
	*/
	virtual EffectNode* GetChild(int index) const = 0;

	/**
	@brief	���ʕ`��p�����[�^�[���擾����B
	*/
	virtual EffectBasicRenderParameter GetBasicRenderParameter() = 0;

	/**
	@brief	���ʕ`��p�����[�^�[��ݒ肷��B
	*/
	virtual void SetBasicRenderParameter(EffectBasicRenderParameter param) = 0;

};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_EFFECT_H__
