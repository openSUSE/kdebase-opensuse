var panel = new Panel
if (panelIds.length == 1) {
    // we are the only panel, so set the location for the user
    panel.location = 'bottom'
}

panel.height = screenGeometry(panel.screen).height > 1024 ? 35 : 27
var launcher = panel.addWidget("launcher")
launcher.globalShortcut = "Alt+F1";
// begin SUSE branding
panel.addWidget("showdesktop");
var firefox = panel.addWidget("icon");
firefox.writeConfig("Url", "firefox.desktop");
var dolphin = panel.addWidget("icon");
dolphin.writeConfig("Url", "dolphin.desktop");
// end SUSE branding
panel.addWidget("pager")
tasks = panel.addWidget("tasks")
panel.addWidget("systemtray")
panel.addWidget("digital-clock")

tasks.currentConfigGroup = new Array("Launchers")
tasks.writeConfig("browser", "preferred://browser, , , ")
tasks.writeConfig("filemanager", "preferred://filemanager, , , ")
