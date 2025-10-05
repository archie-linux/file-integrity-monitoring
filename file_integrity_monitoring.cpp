#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <unordered_map>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <openssl/sha.h>
#include <nlohmann/json.hpp>
#include <filesystem>

using json = nlohmann::json;

using namespace std;
namespace fs = std::filesystem;


// Function to get current timestamp
string getTimestamp() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream timestamp;
    timestamp << 1900 + ltm->tm_year << "-"
              << 1 + ltm->tm_mon << "-"
              << ltm->tm_mday << " "
              << 1 + ltm->tm_hour << ":"
              << 1 + ltm->tm_min << ":"
              << 1 + ltm->tm_sec;
    return timestamp.str();
}

// SHA256 hash function
string getFileHash(const string &filePath) {
    ifstream file(filePath, ios::binary);
    if (!file) return "";

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    char buffer[8192];
    while (file.read(buffer, sizeof(buffer))) {
        SHA256_Update(&sha256, buffer, file.gcount());
    }
    SHA256_Update(&sha256, buffer, file.gcount()); // Process the remaining bytes
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);

    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << hex << (int)hash[i];
    }
    return ss.str();
}

// Function to list all files in monitored directories
vector<string> listFiles(const vector<string>& directories) {
    vector<string> files;
    for (const auto& dir : directories) {
        try {
            for (const auto& entry : fs::recursive_directory_iterator(dir)) {
                if (fs::is_regular_file(entry.path())) {
                    files.push_back(entry.path().string());
                }
            }
        } catch (const fs::filesystem_error& e) {
            cerr << "Error accessing " << dir << ": " << e.what() << endl;
        }
    }
    return files;
}

// Class to handle the File Integrity Database
class IntegrityDatabase {
public:
    IntegrityDatabase(const string &dbFilePath) : dbFilePath(dbFilePath) {
        // Load existing integrity data if any
        ifstream dbFile(dbFilePath);
        if (dbFile) {
            dbFile >> db;  // Parse JSON database into memory
        }
    }

    // Save integrity data back to file
    void save() {
        ofstream dbFile(dbFilePath);
        dbFile << db;
    }

    // Check and update the file hash
    bool checkFileIntegrity(const string &filePath) {
        std::cout << "[DEBUG] File Path: " << filePath << std::endl;
        string currentHash = getFileHash(filePath);
        if (currentHash.empty()) return false;
        // Check if file exists in database
        if (db.contains(filePath)) {
            string storedHash = db[filePath].get<string>();
            // std::cout << "[DEBUG] Stored hash: " << storedHash << std::endl;

            if (storedHash != currentHash) {
                // File has changed
                std::cout << "[WARNING] File modified: " << filePath << std::endl;
                db[filePath] = currentHash;  // Update hash in DB
                return true;  // Modification detected
            }
        } else {
            // File is new, add to database
            std::cout << "[INFO] New file detected, adding to database: " << filePath << std::endl;
            db[filePath] = currentHash;
        }
        // std::cout << "[INFO] No modification detected for file: " << filePath << std::endl;
        return false;  // No modification
    }

private:
    json db;  // JSON object to store file paths and hashes
    string dbFilePath;  // Path to the file where the database is stored
};

// Function to log events (example)
void logEvent(const string &message) {
    cout << getTimestamp() << " - " << message << endl;
}

// ThreadPool class to manage a pool of worker threads
class ThreadPool {
public:
    ThreadPool(size_t numThreads) {
        start(numThreads);
    }

    ~ThreadPool() {
        stop();
    }

    // Enqueue a task for execution
    template <class F>
    void enqueue(F&& f) {
        {
            unique_lock<mutex> lock{eventMutex};
            tasks.push(forward<F>(f));
        }
        eventVar.notify_one();
    }

private:
    vector<thread> threads;
    queue<function<void()>> tasks;
    mutex eventMutex;
    condition_variable eventVar;

    // Start the thread pool
    void start(size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock{eventMutex};
                        eventVar.wait(lock, [this] { return !tasks.empty(); });
                        task = move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    // Stop the thread pool
    void stop() {
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
};

// Function to monitor a single file continuously using a thread
void monitorFile(const string &filePath, IntegrityDatabase &db) {
    if (db.checkFileIntegrity(filePath)) {
        string message = "ALERT: Unauthorized modification detected in " + filePath;
        logEvent(message);  // Log the event
    }
}

// Function to monitor multiple files concurrently using a thread pool
void monitorFiles(const vector<string> &filesToMonitor, IntegrityDatabase &db, ThreadPool &pool) {
    for (const auto& file : filesToMonitor) {
        pool.enqueue([file, &db] {
            monitorFile(file, db);
        });
    }
}

int main() {
    // Path to the integrity database (where file hashes are stored)
    string dbFilePath = "file_integrity_db.json";
    IntegrityDatabase db(dbFilePath);

    vector<string> directories = {"./dummy_dir1/", "./dummy_dir2/"};  // Add more paths if needed

    // List all files before monitoring
    vector<string> filesToMonitor = listFiles(directories);

    // Start a ThreadPool with 4 threads
    ThreadPool pool(4);

    // Start monitoring files using the thread pool
    while (true) {
        monitorFiles(filesToMonitor, db, pool);
        this_thread::sleep_for(chrono::seconds(5));  // Check every 5 seconds
    }

    // Save the integrity database before exiting
    db.save();

    return 0;
}
