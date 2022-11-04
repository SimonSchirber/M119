#Pong game design followed/inspired from from Tech with Tim tutorial at https://www.youtube.com/watch?v=vVGTZlnnX3U&t=345s
import pygame
import time
import asyncio
import struct
from bleak import BleakClient


pygame.init()
WIDTH, HEIGHT = 1000, 500
WIN = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Pong")
#Frames per second max render
FPS = 60

##consts for window
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
PADDLE_WIDTH, PADDLE_HEIGHT = 20, 100
BALL_RADIUS = 8
WINNING_SCORE = 10
SCORE_FONT = pygame.font.SysFont("comicsans", 50)

##########BLE Connections###########
ADDRESS_P1 = ("58:BF:25:9B:35:52")
p1_paired = False
char_1 = 0
service_1 = 0

ADDRESS_P2 = ("")
char_2 = 0
service_2 = 0

def byteToFloat(floatname, floatvalue):
    #use struct to convert byte to float in c notation
    [floatvalue] = struct.unpack('f', floatvalue)
    #print to 3 decimals the value of the float
    print(f'{floatname} :{floatvalue:.2f}')
    return floatvalue
async def test(client):
    print("client called")
    comb_byte_array = await client.read_gatt_char(char_1.uuid)
    float1 = comb_byte_array[0:4]
    byteToFloat("Ay", float1)

async def startBLE(address1):
    global char_1, service_1
    async with BleakClient(address1) as client1:    
        for service in client1.services:   
            for char in service.characteristics:
                if (("read" in char.properties) and ("write" in char.properties)):
                    print("Connected To Player 1")
                    char_1 = char
                    service_1 = service
                    # for i in range(50):
                    #     await test(client1)
                    #     try: 
                    #         comb_byte_array = await client1.read_gatt_char(char_1.uuid)
                    #         float1 = comb_byte_array[0:4]
                    #         byteToFloat("Ay", float1)
                    #     except:
                    #         continue
                    #     time.sleep(.1)
        await main(client1)

class Paddle:
    PADDLE_COLOR = WHITE
    VEL = 15
    def __init__(self, x, y, width, height):
        self.x = self.original_x = x
        self.y = self.original_y = y
        self.width = width
        self.height = height
    
    def draw(self, win):
        pygame.draw.rect(win, self.PADDLE_COLOR, (self.x, self.y, self.width, self.height))
    
    def move(self, BLE = False, up = True, multiplier = 1):
        if (up or BLE):
            self.y -= round(self.VEL*multiplier)
        else:    
            self.y += round(self.VEL*multiplier)
    
    def reset(self):
        self.x = self.original_x
        self.y = self.original_y

class BALL:
    MAX_VEL = 10
    BALL_COLOR = WHITE

    def __init__(self, x, y, radius):
        self.x = self.original_x = x
        self.y = self.origianl_y = y
        self.radius = radius
        self.x_vel = self.MAX_VEL
        self.y_vel = 0
    
    def draw(self, win):
        pygame.draw.circle(win, self.BALL_COLOR,(self.x, self.y), self.radius)

    def move(self):
        self.x += self.x_vel
        self.y += self.y_vel
    
    def reset(self):
        self.x = self.original_x
        self.y = self.origianl_y
        self.y_vel = 0
        self.x_vel *= -1

def handle_collision(ball, left_paddle, right_paddle):
    #For colision on celiling at same angle, simple change x direction
    if ball.y + ball.radius >= HEIGHT:
        ball.y_vel *= -1
    elif ball.y - ball.radius <= 0:
        ball.y_vel *= -1
    #Paddle Collisions
    #Left Paddle
    if ball.x_vel < 0:
        if (ball.y >= left_paddle.y) and (ball.y <= left_paddle.y + left_paddle.height):
            if ball.x - ball.radius <= left_paddle.x + left_paddle.width:
                ball.x_vel *= -1
                #Check where on the paddle the ball hit, change hit direction accordingly
                middle_y = left_paddle.y + left_paddle.height / 2
                diff_y = middle_y - ball.y
                reduction_factor = (left_paddle.height / 2) / ball.MAX_VEL
                y_vel = diff_y / reduction_factor
                ball.y_vel = -y_vel

    #Right Paddle
    else:
        if (ball.y >= right_paddle.y) and (ball.y <= right_paddle.y + right_paddle.height):
            if ball.x + ball.radius >= right_paddle.x:
                ball.x_vel *= -1
                #Check where on the paddle the ball hit, change hit direction accordingly
                middle_y = right_paddle.y + right_paddle.height / 2
                diff_y = middle_y - ball.y
                reduction_factor = (right_paddle.height / 2) / ball.MAX_VEL
                y_vel = diff_y / reduction_factor
                ball.y_vel = -y_vel

def handle_paddle_movement(keys, vel_1, left_paddle, right_paddle):
    #left paddle
    if keys[pygame.K_w] and (left_paddle.y - left_paddle.VEL >= 0):
        left_paddle.move(up = True)
    if keys[pygame.K_s] and (left_paddle.y + left_paddle.VEL + left_paddle.height <= HEIGHT):
        left_paddle.move(up = False)
    #Player 1: right paddle 
    if (vel_1 > 0) and (right_paddle.y - right_paddle.VEL >= 0):
        right_paddle.move( multiplier = vel_1)
    if (vel_1 < 0) and (right_paddle.y + right_paddle.VEL + right_paddle.height <= HEIGHT):
        right_paddle.move( multiplier = vel_1)
    
def render(win, paddles, ball, right_score, left_score):
    WIN.fill(BLACK)
    left_score_text = SCORE_FONT.render(f"{left_score}", 1, WHITE)
    right_score_text = SCORE_FONT.render(f"{right_score}", 1, WHITE)
    win.blit(left_score_text, (WIDTH//4 - left_score_text.get_width()//2, 20))
    win.blit(right_score_text, (WIDTH * 3/4 - right_score_text.get_width()//2, 20))
    #Draw Paddles
    for paddle in paddles:
        paddle.draw(win)
    #Draw middle lines
    for i in range(10, HEIGHT, HEIGHT//20):
        # do every odd
        if i % 2 == 1:
            continue
        pygame. draw.rect(win, WHITE, (WIDTH//2 - 5, i, 10, HEIGHT//20))
    ball.draw(win)
    pygame.display.update()

async def main(client1):
    run = True
    clock = pygame.time.Clock()
    #Declare Paddles
    left_paddle = Paddle(10, HEIGHT//2 - PADDLE_HEIGHT//2, PADDLE_WIDTH, PADDLE_HEIGHT)
    right_paddle = Paddle(WIDTH - 10 - PADDLE_WIDTH, HEIGHT//2 - PADDLE_HEIGHT//2, PADDLE_WIDTH, PADDLE_HEIGHT)
    ball = BALL(WIDTH // 2, HEIGHT //2, BALL_RADIUS)
    left_score = 0
    right_score = 0
    ####How often to read from BLE to not slow down game
    p1_count = 0
    p1_read_frame = 15
    vel_p1 = 0
    #Main loop that runs during game
    while run:
        #Max FPS
        clock.tick(FPS)
        #Only read BLE every 5 frames
        if p1_count == p1_read_frame:
            comb_byte_array = await client1.read_gatt_char(char_1.uuid)
            vel_p1 = comb_byte_array[0:4]
            vel_p1 = byteToFloat("P1: Ay", vel_p1)
            p1_count = 0
        else:
            p1_count += 1

        render(WIN, [left_paddle, right_paddle], ball, right_score, left_score)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False
                break
            keys = pygame.key.get_pressed()
        handle_paddle_movement(keys, vel_p1, left_paddle, right_paddle)
        ball.move()
        handle_collision(ball, left_paddle, right_paddle)
        if ball.x < 0:
            left_score += 1
            ball.reset()
            time.sleep(2)
            right_paddle.reset()
            left_paddle.reset()
        elif ball.x > WIDTH:
            right_score += 1
            ball.reset()
            time.sleep(2)
            right_paddle.reset()
            left_paddle.reset()
        won = False
        if left_score >= WINNING_SCORE:
            won = True
            win_text = "Left Player Won!"
        elif right_score >= WINNING_SCORE:
            won = True
            win_text = "Right Player Won!"
        if won:
            text = SCORE_FONT.render(win_text, 1, WHITE)
            WIN.blit(text, (WIDTH//2 - text.get_width()//2, HEIGHT//2 - text.get_height()//2))
            pygame.display.update()
            pygame.time.delay(5000)
            ball.reset()
            right_paddle.reset()
            left_paddle.reset()
            left_score = 0
            right_score = 0
    pygame.quit()

if __name__ == '__main__':
    asyncio.run(startBLE(ADDRESS_P1))
    

