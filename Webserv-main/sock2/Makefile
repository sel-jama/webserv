CXX = c++
CXXFLAGS = -Wall -Wextra -Werror 
Name = food.ma

SRC = main.cpp src/config.cpp src/paths.cpp src/infra.cpp src/server.cpp src/utils.cpp src/client.cpp src/location.cpp

OBJ = $(SRC:.cpp=.o)

all: $(Name)

$(Name): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(Name)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(Name)

re: fclean all

.PHONY: all fclean re