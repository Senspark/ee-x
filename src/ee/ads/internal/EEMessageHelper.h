@interface EEMessageHelper : NSObject

- (id _Nonnull)initWithPrefix:(NSString* _Nonnull)prefix
                         adId:(NSString* _Nonnull)adId;

- (NSString* _Nonnull)isLoaded;
- (NSString* _Nonnull)load;
- (NSString* _Nonnull)onLoaded;
- (NSString* _Nonnull)onFailedToLoad;
- (NSString* _Nonnull)show;
- (NSString* _Nonnull)onFailedToShow;
- (NSString* _Nonnull)onClicked;
- (NSString* _Nonnull)onClosed;

- (NSString* _Nonnull)getPosition;
- (NSString* _Nonnull)setPosition;
- (NSString* _Nonnull)getSize;
- (NSString* _Nonnull)setSize;
- (NSString* _Nonnull)isVisible;
- (NSString* _Nonnull)setVisible;

- (NSString* _Nonnull)createInternalAd;
- (NSString* _Nonnull)destroyInternalAd;

@end
