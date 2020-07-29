#ifndef UTILS_H
#define UTILS_H
#include "glog/logging.h"
#include "sys/stat.h"

class Utils
{
public:
    Utils();

    bool isPathExist(const std::string &path);
    int initLog(const std::string &logDirPath);

    /**
     * @brief Ping
     *
     * @param[in] address Address to ping.
     * @param[in] max_attempts Number of attempts to try and ping.
     * @param[out] details Details of failure if one occurs.
     *
     * @return True if responsive, false otherwise.
     *
     * @note { I am redirecting stderr to stdout.  I would recommend
     *         capturing this information separately.}
     */
    bool Ping( const std::string& address,
               const int&         max_attempts,
               std::string&       details );

private:
    template<typename TP> TP str2num(std::string const& value); ///< Convert String to Number
    template<typename TP> std::string num2str(TP const& value); ///< Convert Number to String

    /**
     * @brief Execute Generic Shell Command
     *
     * @param[in]   command Command to execute.
     * @param[out]  output  Shell output.
     * @param[in]   mode read/write access
     *
     * @return 0 for success, 1 otherwise.
     *
    */
    int Execute_Command(const std::string&  command,
                        std::string&        output,
                        const std::string&  mode="r");
};

#endif // UTILS_H
