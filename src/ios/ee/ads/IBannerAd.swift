//
//  IAdView.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation

protocol IBannerAd: IAd {
    var isVisible: Bool { get set }
    var position: CGPoint { get set }
    var size: CGSize { get set }
}
