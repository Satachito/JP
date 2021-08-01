//	Written by Satoru Ogura, Tokyo.
//
import	Cocoa

extension
NSView {
    @IBInspectable var
    backgroundColor: NSColor? {
        get {
            guard let wBC = layer?.backgroundColor else { return nil }
            return NSColor( cgColor: wBC )
        }
        set {
            wantsLayer = true
            layer?.backgroundColor = newValue?.cgColor
        }
    }
}
