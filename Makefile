CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -fsanitize=address -std=c++98
Name = Webserver

SRC = sock2/main.cpp sock2/src/config.cpp sock2/src/paths.cpp sock2/src/infra.cpp sock2/src/server.cpp sock2/src/utils.cpp sock2/src/client.cpp sock2/src/location.cpp \
		Response/Method.cpp Response/Response.cpp Request/Request.cpp Request/ParseRequest.cpp post/post.cpp post/delete.cpp  \
		error/errorPage.cpp cgi/handleCgi.cpp

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
