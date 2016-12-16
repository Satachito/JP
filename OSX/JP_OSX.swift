import Cocoa

func
Warning( _ pMessage: String, _ pInfomative: String = "" ) {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .warning
	w.addButton( withTitle: "OK" )
	w.runModal()
}

func
Error( _ p: Error ) {
	let	w = NSAlert()
	w.messageText = "Error"
	w.informativeText = p.localizedDescription
	w.alertStyle = .critical
	w.addButton( withTitle: "OK" )
	w.runModal()
}

func
Input(
	_ pMessage: String = ""
,	_ pInfomative: String = ""
,	_ pDefault: String = ""
,	_ pPlaceholder: String = ""
) -> String? {
	let	w = NSAlert()
	w.messageText = pMessage
	w.informativeText = pInfomative
	w.alertStyle = .warning
	w.addButton( withTitle: "OK" )
    let wTF = NSTextField( frame: NSRect(x: 0, y: 0, width: 200, height: 24 ) )
    wTF.stringValue = pDefault
    wTF.placeholderString = pPlaceholder
	wTF.translatesAutoresizingMaskIntoConstraints = true
    w.accessoryView = wTF
	return w.runModal() == NSAlertFirstButtonReturn ? wTF.stringValue : nil
}

