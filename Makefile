CXX = g++
CXXFLAGS = -std=c++17 -I/opt/homebrew/opt/openssl@3/include -Wno-deprecated-declarations -Wno-unqualified-std-cast-call


LDFLAGS = -L/opt/homebrew/opt/openssl@3/lib -lcrypto -lssl

TARGET = file_integrity_monitoring
SRC = $(wildcard *.cpp)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(TARGET) dummy_dir{1,2} file_integrity_db.json
