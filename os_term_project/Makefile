# 컴파일러 지정 (GCC 사용)
CC = gcc
# 컴파일러 플래그: 경고 표시, 디버그 정보 포함, C11 표준 사용
CFLAGS = -Wall -g -std=c11
# 링커 플래그 (필요시 추가 라이브러리 링크)
LDFLAGS =

# 소스 파일 목록 (.c 파일)
SRCS = main.c myscheduler.c myqueue.c myprocess.c mygantt.c myevaluator.c
# 오브젝트 파일 목록 (.o 파일, SRCS로부터 자동 생성)
OBJS = $(SRCS:.c=.o)

# 실행 파일 이름
TARGET = scheduler

# 기본 규칙: $(TARGET)을 빌드
all: $(TARGET)

# 실행 파일 생성 규칙
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

# .c 파일을 .o 파일로 컴파일하는 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean 규칙: 빌드된 파일들 삭제
clean:
	rm -f $(OBJS) $(TARGET) core *.core

# .PHONY: 가짜 타겟 지정 (실제 파일 이름과 충돌 방지)
.PHONY: all clean