using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

using EE.Internal;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE {
    public interface IAudioInfo {
        public string Path { get; }
        public float Volume { get; }
    }

    public class AudioInfo : IAudioInfo {
        public string Path { get; }
        public float Volume { get; }

        public AudioInfo(string path, float volume = 1f) {
            Path = path;
            Volume = volume;
        }
    }

    public class DefaultAudioManager : IAudioManager {
        [Serializable]
        private class Data {
            public bool music_enabled;
            public bool sound_enabled;
            public float music_volume;
            public float sound_volume;
        }

        private readonly IDataManager _dataManager;
        private readonly Dictionary<Enum, IAudioInfo> _infos;
        private readonly Dictionary<Enum, AudioClip> _clips;
        private Task<bool> _initializer;
        private bool _initialized;
        private Data _data;
        private float _bgmVolume;

        public bool IsMusicEnabled {
            get => _data.music_enabled;
            set {
                if (_data.music_enabled == value) {
                    return;
                }
                _data.music_enabled = value;
                SaveData();
                UpdateMusic();
            }
        }

        public bool IsSoundEnabled {
            get => _data.sound_enabled;
            set {
                if (_data.sound_enabled == value) {
                    return;
                }
                _data.sound_enabled = value;
                SaveData();
                UpdateSound();
            }
        }

        public float MusicVolume {
            get => _data.music_volume;
            set {
                if (Mathf.Approximately(_data.music_volume, value)) {
                    return;
                }
                _data.music_volume = value;
                SaveData();
                UpdateMusic();
            }
        }

        public float SoundVolume {
            get => _data.sound_volume;
            set {
                if (Mathf.Approximately(_data.sound_volume, value)) {
                    return;
                }
                _data.sound_volume = value;
                SaveData();
                UpdateSound();
            }
        }

        public DefaultAudioManager(
            IDataManager dataManager,
            Dictionary<Enum, IAudioInfo> infos) {
            _dataManager = dataManager;
            _infos = infos;
            _clips = new Dictionary<Enum, AudioClip>();
            _bgmVolume = 1f;
        }

        public Task<bool> Initialize() => _initializer ?? (_initializer = InitializeImpl());

        public void Destroy() {
        }

        private async Task<bool> InitializeImpl() {
            await _dataManager.Initialize();
            LoadData();
            UpdateMusic();
            UpdateSound();
            await Task.WhenAll(_infos.Select(async entry => {
                var clip = await Resources.LoadAsync<AudioClip>(entry.Value.Path) as AudioClip;
                Assert.IsNotNull(clip);
                _clips.Add(entry.Key, clip);
            }));
            _initialized = true;
            return true;
        }

        private void LoadData() {
            _data = _dataManager.Get("ee_audio", new Data {
                music_enabled = true,
                sound_enabled = true,
                music_volume = 1,
                sound_volume = 1
            });
        }

        private void SaveData() {
            _dataManager.Set("ee_audio", _data);
        }

        private void UpdateMusic() {
            var instance = AudioPlayer.Instance;
            instance.IsMusicEnabled = IsMusicEnabled;
            instance.MusicVolume = MusicVolume * _bgmVolume;
        }

        private void UpdateSound() {
            var instance = AudioPlayer.Instance;
            instance.IsSoundEnabled = IsSoundEnabled;
            instance.SoundVolume = SoundVolume;
        }

        public void PlayMusic(Enum id) {
            if (!_initialized) {
                return;
            }
            _bgmVolume = _infos[id].Volume;
            UpdateMusic();
            var clip = _clips[id];
            var instance = AudioPlayer.Instance;
            instance.PlayMusic(clip);
        }

        public void PlaySound(Enum id) {
            if (!_initialized) {
                return;
            }
            var volume = _infos[id].Volume;
            var clip = _clips[id];
            var instance = AudioPlayer.Instance;
            instance.PlaySound(clip, volume);
        }
    }
}