import pykraken as kn

kn.init()
kn.window.create((200, 150), scaled=True)
clock = kn.Clock()

while kn.window.is_open():
    dt = clock.tick(60)
    
    for event in kn.event.get():
        if event.type == kn.KEYDOWN:
            if event.key == kn.K_ESC:
                kn.window.close()

    kn.window.clear((20, 20, 20))
    kn.window.flip()

kn.quit()
