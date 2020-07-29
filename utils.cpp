#include "utils.h"

Utils::Utils()
{

}

int Utils::initLog(const std::string &logPath)
{
    bool isLogDirExist = isPathExist(logPath);
    if (isLogDirExist) {
        LOG(INFO) << "Log Dir Exist: " << logPath;
    }
    else {
        int mkdirstt = mkdir(logPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        LOG(INFO) << "Log Dir created: " << logPath;
    }
    return 0;
}

bool Utils::isPathExist(const std::string &path)
{
    struct stat buffer;
    return (stat (path.c_str(), &buffer)==0);
}

template <typename TP>
TP Utils::str2num(std::string const& value)
{
    std::stringstream sin;
    sin << value;
    TP output;
    sin >> output;
    return output;
}

template <typename TP>
std::string Utils::num2str( TP const& value ){
    std::stringstream sin;
    sin << value;
    return sin.str();
}

int Utils::Execute_Command( const std::string&  command,
                     std::string&        output,
                     const std::string&  mode)
{
    // Create the stringstream
    std::stringstream sout;

    // Run Popen
    FILE *in;
    char buff[512];

    // Test output
    if(!(in = popen(command.c_str(), mode.c_str()))){
        return 1;
    }

    // Parse output
    while(fgets(buff, sizeof(buff), in)!=NULL){
        sout << buff;
    }

    // Close
    int exit_code = pclose(in);

    // set output
    output = sout.str();

    // Return exit code
    return exit_code;
}

bool Utils::Ping( const std::string& address,
           const int&         max_attempts,
           std::string&       details )
{
    // Format a command string
    std::string command = "ping -c " + num2str(max_attempts) + " " + address + " 2>&1";
    std::string output;

    // Execute the ping command
    int code = Execute_Command( command, details );

    return (code == 0);
}
