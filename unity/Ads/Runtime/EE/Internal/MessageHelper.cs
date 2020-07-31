namespace EE.Internal {
    public class MessageHelper {
        private readonly string _prefix;
        private readonly string _adId;

        public MessageHelper(string prefix, string adId) {
            _prefix = prefix;
            _adId = adId;
        }

        public string IsLoaded => $"{_prefix}_isLoaded_{_adId}";
        public string Load => $"{_prefix}_load_{_adId}";
        public string OnLoaded => $"{_prefix}_onLoaded_{_adId}";
        public string OnFailedToLoad => $"{_prefix}_onFailedToLoad_{_adId}";
        public string Show => $"{_prefix}_show_{_adId}";
        public string OnFailedToShow => $"{_prefix}_onFailedToShow_{_adId}";
        public string OnClicked => $"{_prefix}_onClicked_{_adId}";
        public string OnClosed => $"{_prefix}_onClosed_{_adId}";

        public string GetPosition => $"{_prefix}_getPosition_{_adId}";
        public string SetPosition => $"{_prefix}_setPosition_{_adId}";
        public string GetSize => $"{_prefix}_getSize_{_adId}";
        public string SetSize => $"{_prefix}_setSize_{_adId}";
        public string IsVisible => $"{_prefix}_isVisible_{_adId}";
        public string SetVisible => $"{_prefix}_setVisible_{_adId}";

        public string CreateInternalAd => $"{_prefix}_createInternalAd_{_adId}";
        public string DestroyInternalAd => $"{_prefix}_destroyInternalAd_{_adId}";
    }
}