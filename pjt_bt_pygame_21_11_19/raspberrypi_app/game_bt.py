import bluetooth
import pygame as pgm

pgm.init() # 초기화 (반드시 필요)

# 화면 크기 설정
scrn_w = 480 # 가로 크기
scrn_h = 640 # 세로 크기
scrn = pgm.display.set_mode((scrn_w, scrn_h))

# 배경 이미지 불러오기
bg_img = pgm.image.load('background.png')

# 캐릭터 이미지 불러오기
chr_img = pgm.image.load('character.png')
chr_size = chr_img.get_rect().size # get img size
chr_w = chr_size[0] # 캐릭터 가로 크기
chr_h = chr_size[1] # 캐릭터 세로 크기
chr_x = scrn_w / 2 - chr_w / 2  # 캐릭터의 가로 크기
chr_y = scrn_h - chr_h

# 화면 타이틀 설정
pgm.display.set_caption("Game basic") # 게임 이름

# FPS
clock = pgm.time.Clock()

# 이동할 좌표
to_x = 0
to_y = 0

# 이동 속도
chr_speed = 1

# 적 캐릭터 이미지 불러오기
enemy_img = pgm.image.load('enemy.png')
enemy_size = enemy_img.get_rect().size # get img size
enemy_w = enemy_size[0] # 캐릭터 가로 크기
enemy_h = enemy_size[1] # 캐릭터 세로 크기
enemy_x = scrn_w / 2 - enemy_w / 2  # 캐릭터의 가로 크기
enemy_y = scrn_h / 2 - enemy_h / 2

# 방향 키 눌림 여부
key_left = 0
key_right = 0
key_up = 0
key_down = 0

# 폰트 정의
game_font = pgm.font.Font(None, 40)

# 총 시간
tot_time = 60

# 방향키 눌림 여부 확인 변수
key_left = 0
key_right = 0
key_up = 0
key_down = 0

def end_game(pgm):
        while True:
                for event in pgm.event.get():
                        if event.type == pgm.KEYDOWN:
                                pgm.quit()
                                return

def scan_services(names):
		print("Scanning for bluetooth devices: ")
		devices = bluetooth.discover_devices(lookup_names = True)
		number_of_devices = len(devices)
		print(number_of_devices, "devices found")
		ret_names = []
		for addr, name in devices:
			if name in names:
				ret_names.append([name,addr])
		return ret_names

connected_devices = scan_services(['kshee']) # 블루투스 장치 이름
if connected_devices == []:
		print('no devices')
else:
		print(connected_devices)
		port = 1
		sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
		sock.connect((connected_devices[0][1], port))

# 시작 시간
start_ticks = pgm.time.get_ticks()

# 이벤트 루프
running = True # 게임 진행 여부

while running:
        dt = 1
#        dt = clock.tick(60) # 게임화면의 초당 프레임 수 설정.

        #print("fps : " + str(clock.get_fps()))

        for event in pgm.event.get(): # 무조건 적어야 함. (발생한 모든 event에 대해 반복)
                if event.type == pgm.QUIT: # 창 닫힘 이벤트 여부 확인
                        running = False
                
                # elif event.type == pgm.KEYDOWN: # 키 눌렸는지.
                #         if event.key == pgm.K_LEFT: # 캐릭터 왼쪽으로
                #                 key_left = 1
                #         elif event.key == pgm.K_RIGHT:
                #                 key_right = 1
                #         elif event.key == pgm.K_UP:
                #                 key_up = 1
                #         elif event.key == pgm.K_DOWN:
                #                 key_down = 1
                
                elif event.type == pgm.KEYUP:
                        if event.key == pgm.K_LEFT:
                                key_left = 0
                        elif event.key == pgm.K_RIGHT:
                                key_right = 0
                        elif event.key == pgm.K_UP:
                                key_up = 0
                        elif event.key == pgm.K_DOWN:
                                key_down = 0
        
        # 방향키 데이터
        kval = sock.recv(1)[0]
        
        # 데이터에서 방향 추출
        key_up = 1 if kval & 1 > 0 else 0
        key_left = 1 if kval & 2 > 0 else 0
        key_down = 1 if kval & 4 > 0 else 0
        key_right = 1 if kval & 8 > 0 else 0
        
        if key_left == 1:
                chr_x -= chr_speed * dt
        if key_right == 1:
                chr_x += chr_speed * dt
        if key_up == 1:
                chr_y -= chr_speed * dt
        if key_down == 1:
                chr_y += chr_speed * dt
        
        if chr_x < 0:
                chr_x = 0
        elif chr_x >= scrn_w - chr_w:
                chr_x = scrn_w - chr_w - 1
        
        if chr_y < 0:
                chr_y = 0
        elif chr_y >= scrn_h - chr_h:
                chr_y = scrn_h - chr_h - 1  

        # 충돌 처리
        chr_rect = chr_img.get_rect()
        chr_rect.left = chr_x
        chr_rect.top = chr_y

        enemy_rect = enemy_img.get_rect()
        enemy_rect.left = enemy_x
        enemy_rect.top = enemy_y

        scrn.fill((0, 160, 255))
        #scrn.blit(bg_img, (0, 0)) # 배경 그리기     
        scrn.blit(chr_img, ( chr_x, chr_y )) # 캐릭터 그리기
        scrn.blit(enemy_img, ( enemy_x, enemy_y )) # 적 캐릭터 그리기

        if chr_rect.colliderect(enemy_rect):
                print("충돌!!")
                scrn.blit(game_font.render('Collision!', True, (0,0,0)), (int(scrn_w / 2) - 50, int(scrn_h / 4)))
                pgm.display.update()
                pgm.time.delay(2000)
                # pygame 종료
                end_game(pgm)
                running = False
                break
        
        # 타이머 집어 넣기
        #       경과 시간 계산
        elapsed_time = (pgm.time.get_ticks() - start_ticks) / 1000 # ms to s

        timer = game_font.render(str(int(tot_time - elapsed_time)), True, (0,0,0))
        # 글자, True, 색상
        scrn.blit(timer, (10,10))

        # 만약 시간이 0 이하라면 게임 종료
        if tot_time - elapsed_time <= 0:
                print("타임 아웃!")
                scrn.blit(game_font.render('Time Out!', True, (0,0,0)), (int(scrn_w / 2) - 50, int(scrn_h / 4)))
                pgm.display.update()
                pgm.time.delay(2000)
                # pygame 종료
                end_game(pgm)
                running = False
                break

        pgm.display.update() # 게임화면 다시 그리기 (항상 호출할 것)        


print('-종료-')
