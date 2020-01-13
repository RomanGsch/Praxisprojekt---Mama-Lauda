import picamera
import time
import pygame

# Voreinstellungen
WIDTH=1280
HEIGHT=1024
FONTSIZE=50

# Kamera initialisieren
camera = picamera.PiCamera()
camera.vflip = False
camera.hflip = False
camera.brightness = 60

# Bildschirmfenster aufbauen, Hintergrund schwarz, Schrift weiss
pygame.init()
screen = pygame.display.set_mode((WIDTH,HEIGHT))
black = pygame.Color(0, 0, 0)
textcol = pygame.Color(255, 255, 0)
screen.fill(black)

while True:
    # Bild machen, als GIF speichern, gleiche Größe wie Fenster
    camera.start_preview()
    time.sleep(1)
    camera.capture('image.gif', format='gif', resize=(WIDTH,HEIGHT))
    camera.stop_preview()

    # altes Bild löschen
    screen.fill(black)
    pygame.display.update()

    # Bild einlesen und anzeigen
    img = pygame.image.load('image.gif')
    screen.blit(img, (0, 0))

    # Datum und Uhrzeit darüber legen
    font = pygame.font.Font('freesansbold.ttf', FONTSIZE)
    text = time.strftime("%d.%m.%Y um %H:%M:%S Uhr")
    font_surf = font.render(text, True, textcol)
    font_rect = font_surf.get_rect()
    font_rect.left = 100
    font_rect.top = 100
    screen.blit(font_surf, font_rect)
    pygame.display.update()

    # etwas warten
    time.sleep(5)

# aus die Maus
camera.close()
pygame.quit()