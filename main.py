import pykraken as kn

kn.init()
kn.window.create("Kraken Window", 800, 600)

while kn.window.is_open():
    for event in kn.event.get():
        if event.type == kn.KEYDOWN and event.key == kn.K_ESC:
            kn.window.close()
    
    kn.window.clear()
    kn.window.flip()

kn.quit()
