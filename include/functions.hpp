#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
/** Version 1.1 */

/**
 *  __________________________________________
 * |                                          |
 * |   ╭━━━━┳━━━┳╮╱╱╭━━━┳━━━┳━━━┳╮╱╭┳━╮╭━╮    |
 * |   ┃╭╮╭╮┃╭━╮┃┃╱╱┃╭━╮┃╭━━┻╮╭╮┃┃╱┃┃┃╰╯┃┃    |
 * |   ╰╯┃┃╰┫┃╱┃┃┃╱╱┃╰━━┫╰━━╮┃┃┃┃┃╱┃┃╭╮╭╮┃    |
 * |   ╱╱┃┃╱┃┃╱┃┃┃╱╭╋━━╮┃╭━━╯┃┃┃┃┃╱┃┃┃┃┃┃┃    |
 * |   ╱╱┃┃╱┃╰━╯┃╰━╯┃╰━╯┃╰━━┳╯╰╯┃╰━╯┃┃┃┃┃┃    |
 * |   ╱╱╰╯╱╰━━━┻━━━┻━━━┻━━━┻━━━┻━━━┻╯╰╯╰╯    |
 * |__________________________________________|
 * |                                          |
 * | Permission is hereby granted, free of    |
 * | charge, to any person obtaining a copy of|
 * | of this software and accompanying files, |
 * | to use them without restriction,         |
 * | including, without limitation, the       |
 * | rights to use, copy, modify, merge,      |
 * | publish, distribute, sublicense and/or   |
 * | sell copies of the software. The authors |
 * | or copyright holders shall not be liable |
 * | for any claims, damages or other         |
 * | liability, whether in contract, tort or  |
 * | otherwise, arising out of or in          |
 * | connection with the software or your use |
 * | or other dealings with the software.     |
 * |__________________________________________|
 * |   website: tolsedum.ru                   |
 * |   email: tolsedum@gmail.com              |
 * |   email: tolsedum@yandex.ru              |
 * |__________________________________________|
 */

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <ctime>
#include <filesystem>
#include <cmath>
#include <cstring>
#include <sys/stat.h>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>


namespace hashes{
	typedef union uwb {
		unsigned w;
		unsigned char b[4];
	} MD5union;

	typedef unsigned DigestArray[4];
	typedef unsigned(*DgstFctn)(unsigned a[]);
	inline std::string getMD5String(std::string msg);
}

/**
 * Universal functions
 * @author Tolsedum
*/
namespace ufn{

    inline std::runtime_error
        error_in_function_create_file_and_dir("");
    /**
     * @brief Сreate a file with parent directories
     * @author Tolsedum
     * @param path
     * @return If file is created then true else false and description
     *      in var error_in_function_create_file_and_dir
     */
    bool createFileAndDirrs(
        std::string path,
        std::string content = ""
    );

    /**
     * Check upper register in string
     * @author Tolsedum
    */
   bool hasUpperRegister(std::string line);

    /**
     * @brief to lowercase
     * @author Tolsedum
     * @return
     */
   std::string toLower(const std::string &s);

    /**
     * Delete pattern by both sides
     * @author Tolsedum
    */
    std::string trim(std::string patient, char pattern = ' ');
    std::string trim(std::string patient, std::vector<char> pattern);


    struct Converter {
        const std::string &x;
        template <typename Numeric> operator Numeric() { return 0;}
    };
    /**
     * @brief If a type is passed to a number with a
     *  string type, it returns this type. It is advisable
     *  to use it after isNumeric
     * @param numeric
     * @return numeric type if exists
    */
    std::string getNumericType(std::string numeric);

    /**
     * @brief Chekc if string is numeric
     * @param str chekcing string
     * @return bool if is numeric then true else false
     * @author Tolsedum
     */
    bool isNumeric(std::string str);

    inline std::string error_in_converter_function = "";
    inline short has_error_in_converter_function = 0;
    /**
     * @brief String to int
     *      If has error then in has_error_in_int_function
     *      set true and error description in hear error_in_int_function.
     *
     * @param number number by string format
     * @return int
     * @author Tolsedum
     */
    int strToInt(const std::string &number);

    /**
     * @brief String to double
     *      If has error then in has_error_in_int_function
     *      set true and error description in hear error_in_int_function.
     *
     * @param number number by string format
     * @return double
     * @author Tolsedum
     */
    double strToDouble(const std::string &number);

    /**
     * @brief String to float
     *      If has error then in has_error_in_int_function
     *      set true and error description in hear error_in_int_function.
     *
     * @param number number by string format
     * @return float
     * @author Tolsedum
     */
    float strToFloat(const std::string &number);

    /**
     * @brief String to unsigned
     *      If has error then in has_error_in_int_function
     *      set true and error description in hear error_in_int_function.
     *
     * @param number number by string format
     * @return float
     * @author Tolsedum
     */
    unsigned strToUnsigned(const std::string &number);

    /**
     * @brief String to long
     *      If has error then in has_error_in_int_function
     *      set true and error description in hear error_in_int_function.
     *
     * @param number number by string format
     * @return long
     * @author Tolsedum
     */
    long strToLong(const std::string &number);

    /**
     * @brief String to long double
     *      If has error then in has_error_in_int_function
     *      set true and error description in hear error_in_int_function.
     *
     * @param number number by string format
     * @return long double
     * @author Tolsedum
     */
    long double strToLongDouble(const std::string &number);

    /**
     * @brief String to long long
     *      If has error then in has_error_in_int_function
     *      set true and error description in hear error_in_int_function.
     *
     * @param number number by string format
     * @return long long
     * @author Tolsedum
     */
    long long strToLongLong(const std::string &number);

     /**
     * @brief String to unsigned long
     *      If has error then in has_error_in_int_function
     *      set true and error description in hear error_in_int_function.
     *
     * @param number number by string format
     * @return unsigned long
     * @author Tolsedum
     */
    unsigned long strToUnsignedLong(const std::string &number);

    /**
     * @brief String to long long
     *      If has error then in has_error_in_int_function
     *      set true and error description in hear error_in_int_function.
     *
     * @param number number by string format
     * @return long long
     * @author Tolsedum
     */
    unsigned long long strToUnsignedLongLong(
        const std::string &number
    );

    /**
     * exploed string to vector conteiner
     * @author Tolsedum
    */
    std::vector<std::string> exploed(
        const std::string &line,
        const std::string &delimiter
    );

    /**
     * Get file content from file pointer
     * @author Tolsedum
    */
    std::string getFileContent(std::ifstream &file);

    /**
     * Get file content by file name
     * @author Tolsedum
    */
    std::string getFileContent(const std::string fileName);

    /**
     * Delete commtnts teg (#)
     * @author Tolsedum
    */
    std::string deleteComment(const std::string &str);

    /**
     * Hash function md5
     * @author Tolsedum
    */
    std::string md5 (std::string in);

    /**
     * Get the currant date using the format
     * @author Tolsedum
    */
    std::string currentDateTime(
        std::string format = "%Y-%m-%d %H:%M:%S");// 2019-08-22 10:55:23


    unsigned long getFileCreationDate(
        const std::string &file_name
    );

    std::string convertTimestampDateToString(
        unsigned long &timestamp,
        std::string format = "%Y-%m-%d %H:%M:%S" // 2019-08-22 10:55:23
    );

    /**
    * @brief Convert string date to timestam
    * @param date string date
    * @param format date format
    * @return unix time
    */
    unsigned long convertStringDateToTimestamp(
        const std::string &date,
        std::string format = "%Y-%m-%d %H:%M:%S" // 2019-08-22 10:55:23
    );

    std::string getParentDir(const std::string_view dir);

    /**
     * implode Template
     *  @author FGTS45
    */
    template<typename TContainer, typename TData>
    std::string implode (TContainer array , char separator = ',')
    {
        std::string outData;
        for (TData arrData : array){
            outData+=std::to_string(arrData) + separator;
        }
        return outData;
    }

    /**
     * Search existing in array
     * @author Tolsedum
    */
    template<class TConteiner, class TValue>
    bool inArray (TConteiner array, TValue value){
        return std::find(array.begin(), array.end(), value) != array.end();
    }
 };
#endif // !FUNCTIONS_HPP
