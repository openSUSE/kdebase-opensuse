function updateShortcut(widget, containment) {
  if (widget != undefined) {
    if (widget.readConfig('plugin') == 'launcher')
    {
      widget.globalShortcut = "Alt+F1";
    }
  }
}

var template = loadTemplate('org.kde.plasma-desktop.findWidgets');
if (template != undefined && template.findWidgets != undefined) {
  template.findWidgets('icon', updateFirefoxUrl)
}
