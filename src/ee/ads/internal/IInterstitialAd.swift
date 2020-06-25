//
//  IInterstitialAd.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation

protocol IInterstitialAd {
    var isLoaded: Bool { get }
    func load()
    func show()
}
