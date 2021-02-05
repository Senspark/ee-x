//
//  EEFacebookShareDelegate.cpp
//  ee_x_mobile_facebook
//
//  Created by eps on 3/20/18.
//

import FBSDKShareKit

class FacebookSharingDelegate: NSObject, SharingDelegate {
    private let _onSuccess: () -> Void
    private let _onCancel: () -> Void
    private let _onError: (_ error: Error) -> Void

    public init(_ onSuccess: @escaping () -> Void,
                _ onCancel: @escaping () -> Void,
                _ onError: @escaping (_ error: Error) -> Void) {
        _onSuccess = onSuccess
        _onCancel = onCancel
        _onError = onError
    }

    func sharer(_ sharer: Sharing, didCompleteWithResults results: [String: Any]) {
        _onSuccess()
    }

    func sharer(_ sharer: Sharing, didFailWithError error: Error) {
        _onError(error)
    }

    func sharerDidCancel(_ sharer: Sharing) {
        _onCancel()
    }
}
