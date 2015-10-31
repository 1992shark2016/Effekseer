using UnityEngine;
using UnityEngine.Rendering;
using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

#if UNITY_EDITOR
using UnityEditor;
#endif

public class EffekseerSystem : MonoBehaviour
{
	/// <summary>
	/// �V�[���r���[�ɕ`�悷�邩�ǂ���
	/// </summary>
	public bool drawInSceneView = true;

	/// <summary>
	/// �C���X�^���X�̍ő吔
	/// </summary>
	public int maxInstances		= 800;

	/// <summary>
	/// �l�p�`�̍ő吔
	/// </summary>
	public int maxSquares		= 1200;

	/// <summary>
	/// �G�t�F�N�g�̕`�悷��^�C�~���O
	/// </summary>
	public CameraEvent cameraEvent	= CameraEvent.AfterForwardAlpha;

	/// <summary>
	/// Effekseer�̃t�@�C����u���ꏊ
	/// </summary>
	public static string resourcePath
	{
		get {
			return Path.Combine(Application.streamingAssetsPath, "Effekseer");
		}
	}

	/// <summary>
	/// �G�t�F�N�g�̍Đ�
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	/// <param name="location">�Đ��J�n����ʒu</param>
	/// <returns>�Đ������G�t�F�N�g�̃n���h��</returns>
	public static EffekseerHandle PlayEffect(string name, Vector3 location)
	{
		IntPtr effect = Instance._GetEffect(name);
		if (effect != IntPtr.Zero) {
			int handle = Plugin.EffekseerPlayEffect(effect, location.x, location.y, location.z);
			return new EffekseerHandle(handle);
		}
		return new EffekseerHandle(-1);
	}
	
	/// <summary>
	/// �S�G�t�F�N�g�̍Đ���~
	/// </summary>
	public static void StopAllEffects()
	{
		Plugin.EffekseerStopAllEffects();
	}

	/// <summary>
	/// �G�t�F�N�g�̃��[�h
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	public static void LoadEffect(string name)
	{
		Instance._LoadEffect(name);
	}

	/// <summary>
	/// �G�t�F�N�g�̉��
	/// </summary>
	/// <param name="name">�G�t�F�N�g��</param>
	public static void ReleaseEffect(string name)
	{
		Instance._ReleaseEffect(name);
	}

	#region Internal Implimentation
	
	// �V���O���g���̃C���X�^���X
	private static EffekseerSystem _Instance = null;
	public static EffekseerSystem Instance
	{
		get {
			if (_Instance == null) {
				// static�ɖ�����ΒT��
				var system = GameObject.FindObjectOfType<EffekseerSystem>();
				if (system != null) {
					// �L���static�ɃZ�b�g
					_Instance = system;
				} else {
					// ������ΐV�����쐬
					var go = GameObject.Find("Effekseer");
					if (go == null) {
						go = new GameObject("Effekseer");
					}
					_Instance = go.AddComponent<EffekseerSystem>();
				}
			}
			return _Instance;
		}
	}
	
	// ���[�h�����G�t�F�N�g
	private Dictionary<string, IntPtr> effectList = new Dictionary<string, IntPtr>();
	
#if UNITY_EDITOR
	// �z�b�g�����[�h�̑ޔ�p
	private List<string> savedEffectList = new List<string>();
#endif

	// �J�������Ƃ̃����_�[�p�X
	class RenderPath {
		public CommandBuffer commandBuffer;
		public CameraEvent cameraEvent;
		public int renderId;
	};
	private Dictionary<Camera, RenderPath> renderPaths = new Dictionary<Camera, RenderPath>();

	private IntPtr _GetEffect(string name) {
		// �g���q�����O����
		name = Path.GetFileNameWithoutExtension(name);
		
		if (effectList.ContainsKey(name) == false) {
			return effectList[name];
		}
		
		// ���݂��Ȃ������烍�[�h
		return _LoadEffect(name);
	}

	private IntPtr _LoadEffect(string name) {
		if (effectList.ContainsKey(name)) {
			return effectList[name];
		}

		// �g���q�����O����
		name = Path.GetFileNameWithoutExtension(name);

		// �p�X����
		string fullPath = Path.Combine(EffekseerSystem.resourcePath, Path.ChangeExtension(name, "efk"));
		fullPath += "\0";
		
		// UTF16�ɕϊ�
		byte[] bytes = Encoding.Unicode.GetBytes(fullPath);
		
		// ��������������Ƀ}�b�v���ă��[�h�����s
		GCHandle ghc = GCHandle.Alloc(bytes, GCHandleType.Pinned);
		IntPtr effect = Plugin.EffekseerLoadEffect(ghc.AddrOfPinnedObject());
		ghc.Free();
		
		if (effect == IntPtr.Zero) {
			Debug.LogError("[Effekseer] Loading error: " + fullPath);
			return IntPtr.Zero;
		}

		effectList.Add(name, effect);
		return effect;
	}
	
	private void _ReleaseEffect(string name) {
		// �g���q�����O����
		name = Path.GetFileNameWithoutExtension(name);
		
		if (effectList.ContainsKey(name) == false) {
			var effect = effectList[name];
			Plugin.EffekseerReleaseEffect(effect);
			effectList.Remove(name);
		}
	}
	
	void Awake() {
		Plugin.EffekseerInit(maxInstances, maxSquares);
	}
	
	void OnDestroy() {
		foreach (var pair in effectList) {
			Plugin.EffekseerReleaseEffect(pair.Value);
		}
		effectList = null;
		Plugin.EffekseerTerm();
	}

	void OnEnable() {
#if UNITY_EDITOR
		// �z�b�g�����[�h���̓��W���[������
		foreach (var effect in savedEffectList) {
			string[] tokens = effect.Split(',');
			if (tokens.Length == 2) {
				effectList.Add(tokens[0], (IntPtr)ulong.Parse(tokens[1]));
			}
		}
		savedEffectList.Clear();
#endif
		CleanUp();
		Camera.onPreCull += OnPreCullEvent;
	}

	void OnDisable() {
#if UNITY_EDITOR
		// Dictionary�͏�����̂ŕ�����ɂ��đޔ�
		foreach (var pair in effectList) {
			savedEffectList.Add(pair.Key + "," + pair.Value.ToString());
		}
		effectList.Clear();
#endif
		Camera.onPreCull -= OnPreCullEvent;
		CleanUp();
	}

	void CleanUp() {
		// �����_�[�p�X�̔j��
		foreach (var pair in renderPaths) {
			var camera = pair.Key;
			var path = pair.Value;
			if (camera != null) {
				camera.RemoveCommandBuffer(path.cameraEvent, path.commandBuffer);
			}
		}
		renderPaths.Clear();
	}
	
	void FixedUpdate() {
		// 1�t���[���X�V
		Plugin.EffekseerUpdate(1);
	}
	
	void OnPreCullEvent(Camera camera) {
#if UNITY_EDITOR
		if (Array.IndexOf<Camera>(SceneView.GetAllSceneCameras(), camera) >= 0) {
			// �V�[���r���[�̃J�����̓`�F�b�N
			if (this.drawInSceneView == false) {
				return;
			}
		} else if (Camera.current.isActiveAndEnabled == false) {
			// �V�[���r���[�ȊO�̃G�f�B�^�J�����͏��O
			return;
		}
#endif
		RenderPath path;
		if (renderPaths.ContainsKey(camera)) {
			// �����_�[�p�X���L��Ύg��
			path = renderPaths[camera];
		} else {
			// ������΍쐬
			path = new RenderPath();
			path.renderId = renderPaths.Count;
			path.cameraEvent = cameraEvent;
			// �v���O�C���`�悷��R�}���h�o�b�t�@���쐬
			path.commandBuffer = new CommandBuffer();
			path.commandBuffer.IssuePluginEvent(Plugin.EffekseerGetRenderFunc(), path.renderId);
			// �R�}���h�o�b�t�@���J�����ɓo�^
			camera.AddCommandBuffer(path.cameraEvent, path.commandBuffer);
			renderPaths.Add(camera, path);
		}

		// �r���[�֘A�̍s����X�V
		SetCameraMatrix(path.renderId, camera);
		SetProjectionMatrix(path.renderId, camera);
	}

	private void SetProjectionMatrix(int renderId, Camera camera) {
		float[] projectionMatrixArray = Matrix2Array(GL.GetGPUProjectionMatrix(
			camera.projectionMatrix, RenderTexture.active));
		GCHandle ghc = GCHandle.Alloc(projectionMatrixArray, GCHandleType.Pinned);
		EffekseerSystem.Plugin.EffekseerSetProjectionMatrix(renderId, ghc.AddrOfPinnedObject());
		ghc.Free();
	}

	private void SetCameraMatrix(int renderId, Camera camera) {
		float[] cameraMatrixArray = Matrix2Array(camera.worldToCameraMatrix);
		GCHandle ghc = GCHandle.Alloc(cameraMatrixArray, GCHandleType.Pinned);
		Plugin.EffekseerSetCameraMatrix(renderId, ghc.AddrOfPinnedObject());
		ghc.Free();
	}

	private float[] Matrix2Array(Matrix4x4 mat) {
		float[] res = new float[16];
		res[ 0] = mat.m00;
		res[ 1] = mat.m01;
		res[ 2] = mat.m02;
		res[ 3] = mat.m03;
		res[ 4] = mat.m10;
		res[ 5] = mat.m11;
		res[ 6] = mat.m12;
		res[ 7] = mat.m13;
		res[ 8] = mat.m20;
		res[ 9] = mat.m21;
		res[10] = mat.m22;
		res[11] = mat.m23;
		res[12] = mat.m30;
		res[13] = mat.m31;
		res[14] = mat.m32;
		res[15] = mat.m33;
		return res;
	}
	
	public static class Plugin
	{
		#if UNITY_IPHONE
			public const string pluginName = "__Internal";
		#else
			public const string pluginName = "EffekseerUnity";
		#endif

		[DllImport(pluginName)]
		public static extern void EffekseerInit(int maxInstances, int maxSquares);
		
		[DllImport(pluginName)]
		public static extern void EffekseerTerm();
		
		[DllImport(pluginName)]
		public static extern void EffekseerUpdate(float deltaTime);
		
		[DllImport(pluginName)]
		public static extern IntPtr EffekseerGetRenderFunc();
		
		[DllImport(pluginName)]
		public static extern void EffekseerSetProjectionMatrix(int renderId, IntPtr matrix);
	
		[DllImport(pluginName)]
		public static extern void EffekseerSetCameraMatrix(int renderId, IntPtr matrix);
		
		[DllImport(pluginName)]
		public static extern IntPtr EffekseerLoadEffect(IntPtr path);
	
		[DllImport(pluginName)]
		public static extern void EffekseerReleaseEffect(IntPtr effect);
	
		[DllImport(pluginName)]
		public static extern int EffekseerPlayEffect(IntPtr effect, float x, float y, float z);
	
		[DllImport(pluginName)]
		public static extern void EffekseerStopEffect(int handle);
	
		[DllImport(pluginName)]
		public static extern void EffekseerStopAllEffects();
	
		[DllImport(pluginName)]
		public static extern void EffekseerSetShown(int handle, bool shown);
	
		[DllImport(pluginName)]
		public static extern void EffekseerSetPaused(int handle, bool paused);
	
		[DllImport(pluginName)]
		public static extern bool EffekseerExists(int handle);

		[DllImport(pluginName)]
		public static extern void EffekseerSetLocation(int handle, float x, float y, float z);

		[DllImport(pluginName)]
		public static extern void EffekseerSetRotation(int handle, float x, float y, float z, float angle);

		[DllImport(pluginName)]
		public static extern void EffekseerSetScale(int handle, float x, float y, float z);
	}
	#endregion
}

/// <summary>
/// �Đ������G�t�F�N�g�̃C���X�^���X�n���h��
/// </summary>
public struct EffekseerHandle
{
	private int m_handle;
	private bool m_paused;
	private bool m_shown;

	public EffekseerHandle(int handle)
	{
		m_handle = handle;
		m_paused = false;
		m_shown = false;
	}
	
	/// <summary>
	/// �G�t�F�N�g���~����
	/// </summary>
	public void Stop()
	{
		EffekseerSystem.Plugin.EffekseerStopEffect(m_handle);
	}
	
	/// <summary>
	/// �G�t�F�N�g�̈ʒu��ݒ�
	/// </summary>
	/// <param name="location">�ʒu</param>
	public void SetLocation(Vector3 location)
	{
		EffekseerSystem.Plugin.EffekseerSetLocation(m_handle, location.x, location.y, location.z);
	}
	
	/// <summary>
	/// �G�t�F�N�g�̉�]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	public void SetRotation(Quaternion rotation)
	{
		Vector3 axis;
		float angle;
		rotation.ToAngleAxis(out angle, out axis);
		EffekseerSystem.Plugin.EffekseerSetRotation(m_handle, axis.x, axis.y, axis.z, angle * Mathf.Deg2Rad);
	}
	
	/// <summary>
	/// �G�t�F�N�g�̊g�k��ݒ�
	/// </summary>
	/// <param name="scale">�g�k</param>
	public void SetScale(Vector3 scale)
	{
		EffekseerSystem.Plugin.EffekseerSetScale(m_handle, scale.x, scale.y, scale.z);
	}

	/// <summary>
	/// Update���ɍX�V���邩
	/// </summary>
	public bool paused
	{
		set {
			EffekseerSystem.Plugin.EffekseerSetPaused(m_handle, value);
			m_paused = value;
		}
		get {
			return m_paused;
		}
	}
	
	/// <summary>
	/// Draw���ŕ`�悳��邩
	/// </summary>
	public bool shown
	{
		set {
			EffekseerSystem.Plugin.EffekseerSetShown(m_handle, value);
			m_shown = value;
		}
		get {
			return m_shown;
		}
	}
	
	/// <summary>
	/// �n���h�����L�����ǂ���<br/>
	/// <para>true:�L��</para>
	/// <para>false:����</para>
	/// </summary>
	public bool enable
	{
		get {
			return m_handle >= 0;
		}
	}
	
	/// <summary>
	/// �G�t�F�N�g�̃C���X�^���X�����݂��Ă��邩�ǂ���
	/// <para>true:���݂��Ă���</para>
	/// <para>false:�Đ��I���Ŕj���B��������Stop�Œ�~���ꂽ</para>
	/// </summary>
	public bool exists
	{
		get {
			return EffekseerSystem.Plugin.EffekseerExists(m_handle);
		}
	}
}