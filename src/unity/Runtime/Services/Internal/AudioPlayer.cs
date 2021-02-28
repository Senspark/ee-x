using UnityEngine;

namespace EE.Internal {
    [DisallowMultipleComponent]
    public class AudioPlayer : MonoBehaviour {
        private static AudioPlayer _sharedInstance;
        private bool _isMusicEnabled;
        private bool _isSoundEnabled;
        private float _bgmVolume;
        private float _sfxVolume;
        private AudioSource _bgmSource;
        private AudioSource _oneShotSfxSource;

        public static AudioPlayer Instance {
            get {
                if (_sharedInstance == null) {
                    var go = new GameObject("AudioPlayer");
                    _sharedInstance = go.AddComponent<AudioPlayer>();
                    DontDestroyOnLoad(go);
                }
                return _sharedInstance;
            }
        }

        public bool IsMusicEnabled {
            get => _isMusicEnabled;
            set {
                if (_isMusicEnabled == value) {
                    return;
                }
                _isMusicEnabled = value;
                UpdateMusic();
            }
        }

        public bool IsSoundEnabled {
            get => _isSoundEnabled;
            set {
                if (_isSoundEnabled == value) {
                    return;
                }
                _isSoundEnabled = value;
                UpdateSound();
            }
        }

        public float MusicVolume {
            get => _bgmVolume;
            set {
                if (Mathf.Approximately(_bgmVolume, value)) {
                    return;
                }
                _bgmVolume = value;
            }
        }

        public float SoundVolume {
            get => _sfxVolume;
            set {
                if (Mathf.Approximately(_sfxVolume, value)) {
                    return;
                }
                _sfxVolume = value;
            }
        }

        private void Awake() {
            _bgmSource = gameObject.AddComponent<AudioSource>();
            _bgmSource.loop = true;

            _oneShotSfxSource = gameObject.AddComponent<AudioSource>();
        }

        private void UpdateMusic() {
            _bgmSource.mute = !_isMusicEnabled;
        }

        public void UpdateSound() {
            if (_isSoundEnabled) {
                // OK.
            } else {
            }
        }

        public void PlayMusic(AudioClip clip) {
            if (clip == _bgmSource.clip) {
                return;
            }
            _bgmSource.clip = clip;
            _bgmSource.Play();
        }

        public void PlaySound(AudioClip clip, float volume) {
            if (!_isSoundEnabled) {
                return;
            }
            _oneShotSfxSource.PlayOneShot(clip, volume);
        }
    }
}