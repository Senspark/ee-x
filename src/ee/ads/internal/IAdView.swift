//
//  IAdView.swift
//  ee-x-366f64ba
//
//  Created by eps on 6/24/20.
//

import Foundation

protocol IAdView {
    var isLoaded: Bool { get }
    func load()
    var position: CGPoint { get set }
    var size: CGSize { get set }
    var isVisible: Bool { get set }
}
