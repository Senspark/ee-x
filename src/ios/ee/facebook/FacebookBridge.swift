//
//  FacebookBridge.swift
//  ee-x-4abf52fd
//
//  Created by eps on 1/28/21.
//

import FBSDKCoreKit
import FBSDKLoginKit
import FBSDKShareKit
import RxSwift

private let kTag = "\(FacebookBridge.self)"
private let kPrefix = "FacebookBridge"
private let kIsLoggedIn = "\(kPrefix)IsLoggedIn"
private let kLogIn = "\(kPrefix)LogIn"
private let kLogOut = "\(kPrefix)LogOut"
private let kGetAccessToken = "\(kPrefix)GetAccessToken"
private let kGraphRequest = "\(kPrefix)GraphRequest"
private let kShareContent = "\(kPrefix)ShareContent"

@objc(EEFacebookBridge)
class FacebookBridge: NSObject, IPlugin {
    private let _bridge: IMessageBridge
    private let _logger: ILogger
    private let _loginManager: LoginManager

    public required init(_ bridge: IMessageBridge, _ logger: ILogger) {
        _bridge = bridge
        _logger = logger
        _loginManager = LoginManager()
        super.init()
        Profile.enableUpdatesOnAccessTokenChange(true)
        Settings.isAutoLogAppEventsEnabled = true
        Settings.isAdvertiserIDCollectionEnabled = true
        registerHandlers()
    }

    public func destroy() {
        deregisterHandlers()
    }

    private func registerHandlers() {
        _bridge.registerHandler(kIsLoggedIn) { _ in
            Utils.toString(self.isLoggedIn)
        }
        _bridge.registerAsyncHandler(kLogIn) { message, resolver in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let permissions = dict["permissions"] as? [String]
            else {
                assert(false, "Invalid argument")
                return
            }
            self.logIn(permissions) { result, error in
                if error != nil {
                    resolver(EEJsonUtils.convertDictionary(toString: [
                        "successful": false,
                        "canceled": false,
                        "errorMessage": error?.localizedDescription ?? ""
                    ]))
                } else if result?.isCancelled ?? true {
                    resolver(EEJsonUtils.convertDictionary(toString: [
                        "successful": false,
                        "canceled": true,
                        "errorMessage": ""
                    ]))
                } else {
                    resolver(EEJsonUtils.convertDictionary(toString: [
                        "successful": true,
                        "canceled": false,
                        "errorMessage": ""
                    ]))
                }
            }
        }
        _bridge.registerAsyncHandler(kLogOut) { _, resolver in
            self.logOut().subscribe(
                onSuccess: { _ in resolver("") },
                onError: { _ in resolver("") })
        }
        _bridge.registerHandler(kGetAccessToken) { _ in
            guard let accessToken = self.accessToken else {
                return ""
            }
            return EEJsonUtils.convertDictionary(toString: [
                "token": accessToken.tokenString,
                "applicationId": accessToken.appID,
                "userId": accessToken.userID
            ])
        }
        _bridge.registerAsyncHandler(kGraphRequest) { message, resolver in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let path = dict["path"] as? String,
                let parameters = dict["parameters"] as? [String: Any]
            else {
                assert(false, "Invalid argument")
                return
            }
            self.graphRequest(path, parameters) {
                _, result, error in
                if error != nil {
                    resolver(EEJsonUtils.convertDictionary(toString: [
                        "successful": false,
                        "response": "",
                        "errorMessage": error?.localizedDescription ?? ""
                    ]))
                } else {
                    resolver(EEJsonUtils.convertDictionary(toString: [
                        "successful": true,
                        "response": EEJsonUtils.convertObject(toString: result ?? []) ?? "",
                        "errorMessage": ""
                    ]))
                }
            }
        }
        _bridge.registerAsyncHandler(kShareContent) { message, resolver in
            let dict = EEJsonUtils.convertString(toDictionary: message)
            guard
                let type = dict["type"] as? Int,
                let url = dict["url"] as? String
            else {
                assert(false, "Invalid argument")
                return
            }
            let callback = FacebookSharingDelegate({
                resolver(EEJsonUtils.convertDictionary(toString: [
                    "successful": true,
                    "canceled": false,
                    "errorMessage": ""
                ]))
            }, {
                resolver(EEJsonUtils.convertDictionary(toString: [
                    "successful": false,
                    "canceled": true,
                    "errorMessage": ""
                ]))
            }, { error in
                resolver(EEJsonUtils.convertDictionary(toString: [
                    "successful": false,
                    "canceled": false,
                    "errorMessage": error.localizedDescription
                ]))
            })
            guard let parsedUrl = URL(string: url) else {
                assert(false, "Invalid url")
                return
            }
            switch type {
            case 0: self.shareLinkContent(parsedUrl, callback)
            case 1: self.sharePhotoContent(parsedUrl, callback)
            case 2: self.shareVideoContent(parsedUrl, callback)
            default: assert(false, "Invalid share type")
            }
        }
    }

    private func deregisterHandlers() {
        _bridge.deregisterHandler(kIsLoggedIn)
        _bridge.deregisterHandler(kLogIn)
        _bridge.deregisterHandler(kLogOut)
        _bridge.deregisterHandler(kGetAccessToken)
        _bridge.deregisterHandler(kGraphRequest)
        _bridge.deregisterHandler(kShareContent)
    }

    private var isLoggedIn: Bool {
        guard let token = AccessToken.current else {
            return false
        }
        return !token.isExpired
    }

    private func logIn(
        _ permission: [String],
        _ callback: @escaping LoginManagerLoginResultBlock) {
        Thread.runOnMainThread {
            let rootView = Utils.getCurrentRootViewController()
            self._loginManager.logIn(
                permissions: permission,
                from: rootView,
                handler: callback)
        }
    }

    private func logOut() -> Single<Bool> {
        return Single<Bool>.create { single in
            Thread.runOnMainThread {
                self._loginManager.logOut()
                single(.success(true))
            }
            return Disposables.create()
        }.subscribeOn(MainScheduler())
    }

    private var accessToken: AccessToken? {
        return AccessToken.current
    }

    private func graphRequest(
        _ path: String,
        _ parameters: [String: Any],
        _ callback: @escaping GraphRequestBlock) {
        Thread.runOnMainThread {
            GraphRequest(
                graphPath: path,
                parameters: parameters,
                httpMethod: HTTPMethod.get)
                .start(completionHandler: callback)
        }
    }

    private func shareLinkContent(_ url: URL, _ callback: SharingDelegate) {
        Thread.runOnMainThread {
            let escapedUrl = URL(string: url.absoluteString.addingPercentEncoding(withAllowedCharacters: CharacterSet.urlQueryAllowed) ?? "") ?? url
            let content = ShareLinkContent()
            content.contentURL = escapedUrl
            self.shareContent(content, callback)
        }
    }

    private func sharePhotoContent(_ url: URL, _ callback: SharingDelegate) {
        Thread.runOnMainThread {
            let photo = SharePhoto(imageURL: url, userGenerated: true)
            let content = SharePhotoContent()
            content.photos = [photo]
            self.shareContent(content, callback)
        }
    }

    private func shareVideoContent(_ url: URL, _ callback: SharingDelegate) {
        Thread.runOnMainThread {
            let video = ShareVideo(videoURL: url)
            let content = ShareVideoContent()
            content.video = video
            self.shareContent(content, callback)
        }
    }

    private func shareContent(_ content: SharingContent, _ callback: SharingDelegate) {
        Thread.runOnMainThread {
            let rootView = Utils.getCurrentRootViewController()
            ShareDialog(
                fromViewController: rootView,
                content: content,
                delegate: callback).show()
        }
    }
}
