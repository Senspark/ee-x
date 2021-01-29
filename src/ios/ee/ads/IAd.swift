//
//  IAd.swift
//  ee-x-4abf52fd
//
//  Created by eps on 1/29/21.
//

protocol IAd {
    func destroy()
    var isLoaded: Bool { get }
    func load()
}
