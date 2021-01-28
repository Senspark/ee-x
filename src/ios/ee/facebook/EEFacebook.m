//
//  Created by Zinge on 7/4/16.
//
//

/*

- (void)registerNotifications {
    [[NSNotificationCenter defaultCenter]
        addObserverForName:FBSDKAccessTokenDidChangeNotification
                    object:nil
                     queue:[NSOperationQueue mainQueue]
                usingBlock:^(NSNotification* notification) {
                    [self accessTokenDidChange:notification];
                }];

    [[NSNotificationCenter defaultCenter]
        addObserverForName:FBSDKProfileDidChangeNotification
                    object:nil
                     queue:[NSOperationQueue mainQueue]
                usingBlock:^(NSNotification* notification) {
                    [self profileDidChange:notification];
                }];
}

- (BOOL)application:(UIApplication*)application
               open:(NSURL*)url
            options:(NSDictionary<UIApplicationOpenURLOptionsKey, id>*)options {
    return [[FBSDKApplicationDelegate sharedInstance] application:application
                                                          openURL:url
                                                          options:options];
}

- (BOOL)application:(UIApplication*)application
                 open:(NSURL*)url
    sourceApplication:(NSString*)sourceApplication
           annotation:(id)annotation {
    return
        [[FBSDKApplicationDelegate sharedInstance] application:application
                                                       openURL:url
                                             sourceApplication:sourceApplication
                                                    annotation:annotation];
}

- (void)accessTokenDidChange:(NSNotification*)notification {
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, [notification userInfo]);
}

- (void)profileDidChange:(NSNotification*)notification {
    FBSDKProfile* profile = [FBSDKProfile currentProfile];
    NSLog(@"%s: %@", __PRETTY_FUNCTION__, profile);
    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    if (profile != nil) {
        [dict setObject:[profile userID] forKey:@"userId"];
        [dict setObject:[profile firstName] forKey:@"firstName"];
        [dict setObject:[profile middleName] ?: @"" forKey:@"middleName"];
        [dict setObject:[profile lastName] forKey:@"lastName"];
        [dict setObject:[profile name] forKey:@"name"];
        [dict setObject:[[profile
                            imageURLForPictureMode:FBSDKProfilePictureModeSquare
                                              size:CGSizeMake(128, 128)]
                            absoluteString]
                 forKey:@"picture"];
    }

    [bridge_ callCpp:k__onProfileChanged //
                    :[EEJsonUtils convertDictionaryToString:dict]];
}

- (NSString* _Nonnull)sendRequest:(NSString* _Nonnull)message_ {
    NSDictionary* dict = [EEJsonUtils convertStringToDictionary:message_];
    int tag = [[dict objectForKey:@"tag"] intValue];
    int actionType = [[dict objectForKey:@"actionType"] intValue];
    int filter = [[dict objectForKey:@"filter"] intValue];
    NSString* title = [dict objectForKey:@"title"];
    NSArray* recipients = [dict objectForKey:@"recipients"];
    NSString* objectId = [dict objectForKey:@"objectId"];
    NSString* data = [dict objectForKey:@"data"];
    NSString* message = [dict objectForKey:@"message"];
    [self sendRequest:(FBSDKGameRequestActionType)actionType
               filter:(FBSDKGameRequestFilter)filter
                title:title
           recipients:recipients
             objectId:objectId
                 data:data
              message:message
             delegate:[EEFacebookRequestDelegate delegateWithBridge:bridge_
                                                                tag:tag]];
    return @"";
}

- (void)sendRequest:(FBSDKGameRequestActionType)actionType
             filter:(FBSDKGameRequestFilter)filter
              title:(NSString* _Nullable)title
         recipients:(NSArray* _Nullable)recipients
           objectId:(NSString* _Nullable)objectId
               data:(NSString* _Nullable)data
            message:(NSString* _Nonnull)message
           delegate:(id<FBSDKGameRequestDialogDelegate> _Nonnull)delegate {
    FBSDKGameRequestContent* content =
        [[[FBSDKGameRequestContent alloc] init] autorelease];
    [content setActionType:actionType];
    [content setFilters:filter];
    [content setObjectID:objectId];
    [content setTitle:title];
    [content setRecipients:recipients];
    [content setData:data];
    [content setMessage:message];
    [FBSDKGameRequestDialog showWithContent:content delegate:[delegate retain]];
}
 
@end

*/
