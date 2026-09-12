# Compilador e flags de compilação
CXX = g++
CXXFLAGS = -Wall -Wextra -O3 -std=c++17

# Nome do executável final
EXEC = tsp

# Lista de todos os arquivos .cpp do projeto
SRCS = teste.cpp Data.cpp 

# Transforma os arquivos .cpp em .o (arquivos objeto)
OBJS = $(SRCS:.cpp=.o)

# Regra principal: compila o executável
all: $(EXEC)

# Regra para gerar o executável a partir dos objetos
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

# Regra genérica para compilar cada .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados (.o e o executável)
clean:
	rm -f $(OBJS) $(EXEC)

# Regra para recompilar tudo do zero
re: clean all

.PHONY: all clean re