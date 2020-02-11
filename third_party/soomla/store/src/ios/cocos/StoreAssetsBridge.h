//

//


#import <Foundation/Foundation.h>
#import "IStoreAssets.h"

@interface StoreAssetsBridge : NSObject <IStoreAssets>{
    int version;
    NSMutableArray* virtualCurrenciesArray;
    NSMutableArray* virtualGoodsArray;
    NSMutableArray* virtualCurrencyPacksArray;
    NSMutableArray* virtualCategoriesArray;
}

+ (StoreAssetsBridge *)sharedInstance;
- (void)initializeWithStoreAssetsDict:(NSDictionary*)storeAssetsDict andVersion:(int)oVersion;

@end
