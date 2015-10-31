using UnityEngine;
using System;
using System.Collections;

/// <summary>
/// �G�t�F�N�g�̔�����
/// </summary>
public class EffekseerEmitter : MonoBehaviour
{
	/// <summary>
	/// �G�t�F�N�g��
	/// </summary>
	public string effectName;

	/// <summary>
	/// Start���ɍĐ��J�n���邩�ǂ���
	/// </summary>
	public bool playOnStart = false;

	/// <summary>
	/// ���[�v�Đ����邩�ǂ���
	/// </summary>
	public bool loop = false;

	/// <summary>
	/// �ێ����Ă���n���h��
	/// </summary>
	private EffekseerHandle? handle;
	
	/// <summary>
	/// �G�t�F�N�g���Đ�
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	public void Play(string name)
	{
		effectName = name;
		Play();
	}
	
	/// <summary>
	/// �ݒ肳��Ă���G�t�F�N�g���Đ�
	/// </summary>
	public void Play()
	{
		handle = EffekseerSystem.PlayEffect(effectName, transform.position);
		UpdateTransform();
	}
	
	/// <summary>
	/// �Đ����̃G�t�F�N�g�̒�~
	/// </summary>
	public void Stop()
	{
		if (handle.HasValue) {
			handle.Value.Stop();
			handle = null;
		}
	}
	
	/// <summary>
	/// �Đ����̃G�t�F�N�g��ێ����Ă��邩
	/// </summary>
	public bool active
	{
		get {
			return handle.HasValue && handle.Value.exists;
		}
	}
	
	#region Internal Implimentation

	void Start()
	{
		EffekseerSystem.LoadEffect(effectName);
		if (playOnStart) {
			Play();
		}
	}
	
	void OnDestroy()
	{
		Stop();
	}
	
	void Update()
	{
		if (handle.HasValue) {
			if (handle.Value.exists) {
				UpdateTransform();
			} else if (loop) {
				Play();
			} else {
				Stop();
			}
		}
	}

	void UpdateTransform() {
		if (handle.HasValue) {
			handle.Value.SetLocation(transform.position);
			handle.Value.SetRotation(transform.rotation);
			handle.Value.SetScale(transform.localScale);
		}
	}

	#endregion
}
