
#import <Foundation/Foundation.h>


@interface DomainFactory : NSObject
+ (id)sharedDomainFactory;
- (void)registerCreatorForKey: (NSString *)key withBlock: (id (^)(NSDictionary *dict)) creator;
- (id)createWithDict: (NSDictionary *)dict;
@end
