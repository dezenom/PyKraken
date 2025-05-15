import pykraken as kn

kn.init()
kn.window.create("Kraken Window", 800, 600)
clock = kn.Clock()

while kn.window.is_open():
    dt = clock.tick(240)
    
    for event in kn.event.get():
        if event.type == kn.KEYDOWN and event.key == kn.K_ESC:
            kn.window.close()
    
    kn.window.clear()
    kn.window.flip()

kn.quit()
