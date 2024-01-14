NAME = send

SRC = UsbToArduino.cpp

OBJ = $(SRC:.cpp=.o)

FLAGS = -Wall -Wextra -Werror

LIBS = -lserialport

all: $(NAME)

$(NAME): $(OBJ)
	c++ $(FLAGS) -o $(NAME) $(OBJ) $(LIBS)
	@echo "Compilation done!"
	@echo "Run with ./$(NAME)"
	@echo "Or  with ./$(NAME) -v for verbose mode"

%.o: %.cpp
	c++ $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

extract:
	@echo "Extracting images"
	@rm -rf ./images
	@mkdir -p ./images
	@ffmpeg -i inputvideo.mp4 -vf "select=not(mod(n\,3))" -vsync vfr "images/output_%03d.png"
	@echo "Done!"

convert:
	@echo "Converting to Byte Array format"
	@python3 Img2ByteArray.py
	@echo "Done!"

PHONY: all clean fclean re convert
