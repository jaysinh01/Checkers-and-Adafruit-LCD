CXXFLAGS =	-O0 -g -Wall -fmessage-length=0 -std=gnu++11

OBJS =	CheckersAI.o  Piece.o Player.o AI.o Human.o AImove.o CheckerBoard.o serialport.o



TARGET =	CheckersAI

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

all:	$(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)


