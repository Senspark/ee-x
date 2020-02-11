
#import <Foundation/Foundation.h>

@interface DomainHelper : NSObject
+ (id)sharedDomainHelper;
- (void) registerType: (NSString *)type withClassName: (NSString *)className;
- (void) registerType: (NSString *)type withClassName: (NSString *)className andBlock: (id (^)(NSDictionary *dict)) creator;
- (id) getDomainsFromDictList: (NSArray *)dictList;
- (id) getDictListFromDomains: (NSArray *)domains;
- (NSDictionary *) domainToDict: (NSObject *)domain;
@end
