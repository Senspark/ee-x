using System;

using UnityEngine.Scripting;

namespace EE {
    [Preserve]
    [Service("EE" + nameof(IAudioManager))]
    public interface IAudioManager : IService {
        bool IsMusicEnabled { get; set; }
        bool IsSoundEnabled { get; set; }
        float MusicVolume { get; set; }
        float SoundVolume { get; set; }
        void PlayMusic(Enum id);
        void PlaySound(Enum id);
    }
}