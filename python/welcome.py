'''
A welcome screen for my computer
'''

import wx

class HelloFrame(wx.Frame):
    '''
    a frame that says hi
    '''

    def __init__(self, *args, **kw):
        super(HelloFrame, self).__init__(*args, **kw)

        # create a panel
        panel = wx.Panel(self)

        # put some text with a larger bold font on it
        stext = wx.StaticText(panel, label='hello bro')
        font = stext.GetFont()
        font.PointSize += 10
        font = font.Bold()

        # create a sizer to manage the layout of child widgets
        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(stext, wx.SizerFlags().Border(wx.TOP | wx.LEFT, 25))
        panel.SetSizer(sizer)

        # create a menu bar
        self.makeMenuBar()

        # add a status bar
        self.CreateStatusBar()
        self.SetStatusText('''everything's gud''')

    def makeMenuBar(self):
        '''
        a menu bar is composed of menus, which are composed of menu itme.s
        This method builds a set of menus and binds handlers to be called when the menu item is selected.
        '''

        # make a file menu with hello and exit items
        fileMenu = wx.Menu()

        helloItem = fileMenu.Append(-1, '&Hello...\tCtrl-H', 'help string shown in status bar for this menu item')
        fileMenu.AppendSeparator()

        exitItem = fileMenu.Append(wx.ID_EXIT)

        helpMenu = wx.Menu()
        aboutItem = helpMenu.Append(wx.ID_ABOUT)

        # make the menu bar and add the two menus to it
        # The '&' defines that the next letter is the "mnemonic" for the menu item. On the platforms that support it, thos eletters are underlined and can be triggered from the keyboard.
        menuBar = wx.MenuBar()
        menuBar.Append(fileMenu, "&File")
        menuBar.Append(helpMenu, "&Help")

        # give the menu bar to the frame
        self.SetMenuBar(menuBar)

        # finally, associate a handler function with the EVT_MENU event for each of the menu items
        # that means that when that menu item is activated, the associated handler function will be called
        self.Bind(wx.EVT_MENU, self.OnHello, helloItem)
        self.Bind(wx.EVT_MENU, self.OnExit, exitItem)
        self.Bind(wx.EVT_MENU, self.OnAbout, aboutItem)

    def OnExit(self, event):
        '''close the frame, terminating the program'''
        self.Close(True)
    
    def OnHello(self, event):
        '''say hello to the user'''
        wx.MessageBox("hello from wxpython")

    def OnAbout(self, event):
        '''display an about dialog'''
        wx.MessageBox('this is a wxpython hello world sample', 'about hello world 2', wx.OK | wx.ICON_INFORMATION)

if __name__ == '__main__':
    # when the module is run (not imported) then create the app, the frame, show it, and start the event loop
    app = wx.App()
    frm = HelloFrame(None, title='Welcome to GNU/Linux!')
    frm.Show()
    app.MainLoop()
