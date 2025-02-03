// Copyright (c) 2019-2024  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/snapcatch2
// contact@m2osw.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#pragma once

// catch2
//
#define CATCH_CONFIG_PREFIX_ALL
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#pragma GCC diagnostic ignored "-Wsign-promo"
#include    <catch2/catch_session.hpp>
#include    <catch2/catch_test_macros.hpp>
#include    <catch2/catch_approx.hpp>
#include    <catch2/matchers/catch_matchers.hpp>
#pragma GCC diagnostic pop


// C++
//
#include    <stdexcept>
#include    <fstream>
#include    <iomanip>
#include    <iostream>
#include    <sstream>


// C
//
#include    <string.h>
#include    <unistd.h>


/** \brief Allow for the literals defined by Catch.
 *
 * This statement makes all the literals offered by Catch to be usable in
 * your tests.
 *
 * For floating point compare against a literal, one can do:
 *
 * CATCH_REQUIRE(var == 1.0_a); // equivalent to Approx(1.0)
 *
 * Below we also offer a floating point REQUIRE like so:
 *
 * CATCH_REQUIRE_FLOATING_POINT(var, 1.0);
 *
 * That require uses our implementation of the compare which is limited
 * to an epsilon variance.
 */
using namespace Catch::literals;


/** \brief Namespace declaration.
 *
 * You are expected to reuse this name each time you create a common
 * function, class, etc. For example the following creates a counter
 * which is common to all your tests:
 *
 * \code
 *     namespace SNAP_CATCH2_NAMESPACE
 *     {
 *     
 *     int g_counter = 0;
 *     
 *     }
 * \endcode
 */
#define SNAP_CATCH2_NAMESPACE       unittest



/** \brief The snapcatch2 definitions.
 *
 * This namespace encompasses the extensions we offer to catch2. We reuse
 * these in pretty much all our tests so to make it easy we made it a
 * common header file.
 */
namespace SNAP_CATCH2_NAMESPACE
{



/** \brief Print out progress as a section gets executed.
 *
 * This flag is used along the CATCH_START_SECTION() and CATCH_END_SECTION()
 * macros. The flag gets set whenever the `--progress` command line option
 * is used and the macros print out the name of the section being run
 * (assuming you use our macros).
 *
 * This is very useful to have whenever you have very many and rather
 * slow tests to see which one fails because otherwise you may get
 * some invalid line number/names especially when you have an unexpected
 * exception or some similar error.
 *
 * You are welcome to use the parameter in your own code. Just make sure
 * to use the parenthesis:
 *
 * \code
 *     if(g_progress())
 *     {
 *         std::cout << "we're moving!" << std::endl;
 *     }
 * \endcode
 *
 * \return A read-write reference to the `progress` parameter.
 */
inline bool & g_progress()
{
    static bool progress = false;

    return progress;
}


/** \brief Retrieve the path to the source directory.
 *
 * Some tests need access to the source directory in order to read files
 * such as the debian/changelog to get the current version or some
 * configuration files.
 *
 * This function returns a reference to that path which can be set by
 * the user on the command line using the `--source-dir` option.
 *
 * \return A reference to the source directory.
 */
inline std::string & g_source_dir()
{
    static std::string source_dir = std::string();

    return source_dir;
}


/** \brief Retrieve the path to the binary (build) directory.
 *
 * Some tests need access to the binary directory in order to access
 * generated files including tools that were created by cmake.
 * Those files will be found under this directory.
 *
 * This function returns a reference to that path which can be set by
 * the user on the command line using the `--binary-dir` option.
 *
 * \return A reference to the binary directory.
 */
inline std::string & g_binary_dir()
{
    static std::string binary_dir = std::string();

    return binary_dir;
}


/** \brief Retrieve the path to the dist(ribution) directory.
 *
 * Some tests need access to the dist directory where dependencies
 * installed their files. For example, the eventdispatcher message
 * verification process loads files from
 * `/usr/share/eventdispatcher/messages`. This folder is likely
 * to not exist and if it does, it is even less likely to be up to
 * date on a programmer's machine.
 *
 * This function returns a reference to that path which is used
 * to install such files while building all the contribs and
 * main snapwebsites files.
 *
 * The unit test is expected to use the `--dist-dir` option in
 * order to define this directory.
 *
 * \return A reference to the binary directory.
 */
inline std::string & g_dist_dir()
{
    static std::string dist_dir = std::string();

    return dist_dir;
}


/** \brief Retrieve the temporary directory.
 *
 * Many tests make use of input and output files. These are expected
 * to be saved in a temporary. This function returns the path to that
 * directory.
 *
 * The path will have been cleaned up on initialization. It is available
 * in all of our tests, although many use a temporary folder, some do not.
 *
 * \return A reference to the user specified directory.
 */
inline std::string & g_tmp_dir()
{
    static std::string tmp_dir = std::string();

    return tmp_dir;
}


/** \brief Generate a random number.
 *
 * This function uses the rand() function to generate a random number
 * for your test. This is very simple and it can easily be reproduced
 * since we offer the --seed command line option to re-seed your test
 * with the same value.
 *
 * The template expects to be used to fill the number of bits specified
 * in the type. By default, rand() returns less than 32 bits of truely
 * random data. This function makes sure that all the bits are set to
 * some random value.
 *
 * Note that it is extremely unlikely that you get 0 or the maximum
 * value ("-1" for an unsigned number or "-1 >> 1" for a signed number).
 * If you want your test to verify those specific numbers, make sure
 * to include special cases in your code.
 *
 * \note
 * This function takes the result as a parameter in order to determine
 * the template parameter T.
 *
 * \exception std::logic_error
 * If somehow you have a type which size is not handled, then the function
 * emits a logic_error. This may happen if you use intrinsic types which
 * can be 256 or 512 bits and these are not handled by this function.
 *
 * \tparam T  The type of integral number to generate.
 * \param[out] result  The resulting random number.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
template<typename T>
void random(T & result)
{
    int const size((sizeof(T) + 1) / 2);
    switch(size)
    {
    case 1: // 8 and 16 bits
        result = static_cast<T>(rand());
        break;

    case 2: // 32 bits
        result = static_cast<T>(
                (static_cast<std::uint32_t>(rand()) << 16)
              ^ (static_cast<std::uint32_t>(rand()) <<  0));
        break;

    case 4: // 64 bits
        result = static_cast<T>(
               (static_cast<std::uint64_t>(rand()) << 48)
             ^ (static_cast<std::uint64_t>(rand()) << 32)
             ^ (static_cast<std::uint64_t>(rand()) << 16)
             ^ (static_cast<std::uint64_t>(rand()) <<  0));
        break;

    case 8: // 128 bits
        result = static_cast<T>(
               (static_cast<unsigned __int128>(rand()) << 112)
             ^ (static_cast<unsigned __int128>(rand()) <<  96)
             ^ (static_cast<unsigned __int128>(rand()) <<  80)
             ^ (static_cast<unsigned __int128>(rand()) <<  64)
             ^ (static_cast<unsigned __int128>(rand()) <<  48)
             ^ (static_cast<unsigned __int128>(rand()) <<  32)
             ^ (static_cast<unsigned __int128>(rand()) <<  16)
             ^ (static_cast<unsigned __int128>(rand()) <<   0));
        break;

    default:
        CATCH_REQUIRE("unsupported type for random()" == nullptr);
        throw std::logic_error("unsupported type for random()");

    }
}
#pragma GCC diagnostic pop


enum class character_t
{
    CHARACTER_ZUNICODE,      // any from 0 to 0x10FFFF
    CHARACTER_ZPLAN0,        // any from 0 to 0x00FFFF
    CHARACTER_UNICODE,       // any from 1 to 0x10FFFF
    CHARACTER_PLAN0,         // any from 1 to 0x00FFFF
    CHARACTER_LETTERS,       // a-z, A-Z
    CHARACTER_ALPHANUMERIC,  // a-z, A-Z, 0-9
    CHARACTER_DIGITS,        // 0-9
    CHARACTER_ASCII,         // 0x20 to 0x7E
    CHARACTER_LABEL,         // a-z, A-Z, 0-9, _
};


inline char32_t random_char(character_t category)
{
    constexpr std::size_t const SURROGATES_COUNT(0xE000 - 0xD800);
    char32_t result(U'\0');
    random(result);
    switch(category)
    {
    case character_t::CHARACTER_ZUNICODE:
        result %= 0x110000 - SURROGATES_COUNT;
        break;

    case character_t::CHARACTER_ZPLAN0:
        result %= 0x010000 - SURROGATES_COUNT;
        break;

    case character_t::CHARACTER_UNICODE:
        result = result % (0x110000 - SURROGATES_COUNT - 1) + 1;
        break;

    case character_t::CHARACTER_PLAN0:
        result = result % (0x010000 - SURROGATES_COUNT - 1) + 1;
        break;

    case character_t::CHARACTER_LETTERS:
        result = result % (26 * 2) + 'A';
        if(result > 'Z')
        {
            // lowercase
            //
            result += 'a' - 'Z' - 1;
        }
        break;

    case character_t::CHARACTER_ALPHANUMERIC:
        result = result % (26 * 2 + 10) + '0';
        if(result > '9')
        {
            // uppercase
            //
            result += 'A' - '9' - 1;
            if(result > 'Z')
            {
                // lowercase
                //
                result += 'a' - 'Z' - 1;
            }
        }
        break;

    case character_t::CHARACTER_DIGITS:
        result = result % 10 + '0';
        break;

    case character_t::CHARACTER_ASCII:
        result = result % ('~' - ' ' + 1) + ' ';
        break;

    case character_t::CHARACTER_LABEL:
        result = result % (26 * 2 + 10 + 1) + '0';
        if(result > '9')
        {
            // uppercase
            //
            result += 'A' - '9' - 1;
            if(result > 'Z')
            {
                // lowercase
                //
                result += 'a' - 'Z' - 1;
                if(result > 'z')
                {
                    result = '_';
                }
            }
        }
        break;

    }

    if(result >= 0xD800)
    {
        // skip the surrogates (not valid char32_t values)
        //
        result += SURROGATES_COUNT;
    }

    return result;
}


/** \brief Convert a char32_t to a UTF-8 chain of bytes.
 *
 * This function converts a char32_t in a string of UTF-8 bytes. If the input
 * is an invalid character (larger than 0x10FFFF) then nothing happens.
 *
 * This function is expected to only be used in tests. In your code,
 * make sure to use the libutf8::to_u8string().
 *
 * \param[in,out] out  The output string where bytes get added.
 * \param[in] wc  The input wide character.
 */
inline void wctombs(std::string & out, char32_t wc)
{
    if(wc < 0x80)
    {
        out += static_cast<char>(wc);
    }
    else if(wc < 0x800)
    {
        out += static_cast<char>((wc >> 6) | 0xC0);
        out += static_cast<char>((wc & 0x3F) | 0x80);
    }
    else if(wc < 0x10000)
    {
        out += static_cast<char>((wc >> 12) | 0xE0);
        out += static_cast<char>(((wc >> 6) & 0x3F) | 0x80);
        out += static_cast<char>((wc & 0x3F) | 0x80);
    }
    else if(wc < 0x110000)
    {
        out += static_cast<char>((wc >> 18) | 0xF0);
        out += static_cast<char>(((wc >> 12) & 0x3F) | 0x80);
        out += static_cast<char>(((wc >> 6) & 0x3F) | 0x80);
        out += static_cast<char>((wc & 0x3F) | 0x80);
    }
}


inline std::string random_string(
          std::size_t length_min
        , std::size_t length_max // inclusive
        , character_t category = character_t::CHARACTER_ASCII)
{
    std::size_t length(0);
    random(length);
    length = length % (length_max + 1 - length_min) + length_min;

    std::string result;
    for(std::size_t i(0); i < length; ++i)
    {
        char32_t c(random_char(category));

        // labels cannot start with a digit
        //
        while(result.empty()
           && category == character_t::CHARACTER_LABEL
           && c >= '0'
           && c <= '9')
        {
            c = random_char(category);
        }

        wctombs(result, c);
    }

    return result;
}


inline std::vector<std::uint8_t> random_buffer(
          std::size_t size_min  // inclusive
        , std::size_t size_max) // inclusive
{
    std::size_t size(0);
    random(size);
    size = size % (size_max + 1 - size_min) + size_min;

    std::vector<std::uint8_t> result(size);
    for(std::size_t i(0); i < size; ++i)
    {
        random(result[i]);
    }

    return result;
}


namespace detail
{

inline void init_tmp_dir(std::string const & project_name)
{
    std::string & path(g_tmp_dir());

    // make sure it's not just "/tmp" since we're about to delete it
    // with `rm -rf ...`
    //
    if(path == "/tmp")
    {
        std::cerr
            << "fatal error: temporary directory cannot be \"/tmp\"."
              " It needs to include a sub-directory (\"/tmp/"
            << project_name
            << "\", for example)."
            << std::endl;
        exit(1);
    }

    // default to /tmp/<project-name> if not defined
    //
    if(path.empty())
    {
        // TODO: make sure "project_name" is "clean" (spaces -> '_', slashes?)
        //
        path = "/tmp/" + project_name;
    }

    // delete the directory if it exists
    //
    // this ensure an equivalent state each time we run the tests
    {
        std::stringstream ss;
        ss << "rm -rf \"" << path << "\"";
        if(system(ss.str().c_str()) != 0)
        {
            std::cerr
                << "fatal error: could not delete temporary directory \""
                << path
                << "\".";
            exit(1);
        }
    }

    // create the directory
    {
        std::stringstream ss;
        ss << "mkdir -p \"" << path << "\"";
        if(system(ss.str().c_str()) != 0)
        {
            std::cerr
                << "fatal error: could not create temporary directory \""
                << path
                << "\".";
            exit(1);
        }
    }
}

} // detail namespace


/** \brief Print out information to let programmers know what tests are doing.
 *
 * This flag is used whenever useful information could be outputted. The
 * flag let the test know whether to print that information or not.
 *
 * You are welcome to use the parameter in your own code. Just make sure
 * to use the parenthesis:
 *
 * \code
 *     if(g_verbose())
 *     {
 *         std::cout << "More info about this test!" << std::endl;
 *     }
 * \endcode
 *
 * \return A read-write reference to the `verbose` parameter.
 */
inline bool & g_verbose()
{
    static bool verbose = false;

    return verbose;
}


#ifdef CATCH_CONFIG_RUNNER
/** \brief The main function to initialize and run the unit tests.
 *
 * This inline function initializes and runs the snapcatch2 tests.
 *
 * The \p init_callback parameter is used to initialize your test before
 * we parse the command line. Use the \p callback function to do further
 * initialization after the command line is parsed and basic functions
 * run (i.e. the `--version` command line option will run and the
 * function exit before \p callback gets called. So when the user uses
 * that command line option, you do not get \p callback called, however,
 * \p init_callback will always be called).
 *
 * We do it this way because the function will try/catch std::exception
 * so if an exception occurs in your \p init_callback, it will always
 * be captured and a message printed out before the application
 * exits cleanly with an error code (i.e. `exit(1)`).
 *
 * The \p init_callback can be set to a nullptr if you do not need to
 * initialize anything futher early on.
 *
 * One thing we use the \p init_callback for is to prevent our
 * libexcept exceptions from generating a stack trace because that's
 * very slow and in a test which generate thousands if not million
 * of exception cases, it makes it really very slow.
 *
 * \code
 *     snap_catch2_main(
 *           "libutf8"
 *         , "1.2.3"
 *         , argc
 *         , argv
 *         , []() { libexcept::set_collect_stack(libexcept::collect_stack_t::COLLECT_STACK_NO); }
 *     );
 * \endcode
 *
 * The \p add_user_options parameter is a pointer to a function which
 * adds command line options supported by your test. This pointer can
 * be set to nullptr, For example, if you wanted to support a path to
 * a temporary directory, you would write:
 *
 * \code
 *     namespace SNAP_CATCH2_NAMESPACE
 *     {
 *
 *     std::string g_conf_dir = std::string("/etc/config");
 *
 *     Catch::Clara::Parser add_command_line_options(Catch::Clara::Parser const & cli)
 *     {
 *         return cli
 *              | Catch::Clara::Opt(g_conf_dir, "conf-dir")
 *                   ["--conf-dir"]
 *                   ("a path to a configuration directory used by the test.");
 *     }
 *
 *     }
 *
 *     ...snip...
 *
 *     snap_catch2_main(
 *           "libutf8"
 *         , "1.2.3"
 *         , argc
 *         , argv
 *         , nullptr
 *         , &add_command_line_options
 *     );
 * \endcode
 *
 * \warning
 * It is possible to add a one letter command line option such as ["-t"],
 * however, catch2 already uses most of these options. The `-t` is used
 * to list tags, for example.
 *
 * The \p callback function gets called just before the tests run.
 * In other words, we give you one more chance to verify command line
 * parameters or do additional initilalizations, now that you have access
 * to the user defined command line parameters. We pass the session to
 * the function, although in many cases you will not use it because your
 * parameters are already set as expected.
 *
 * \code
 *     namespace SNAP_CATCH2_NAMESPACE
 *     {
 *
 *     int finish_init(Catch::Session & session)
 *     {
 *         std::string cmd("rm -rf ");
 *         cmd += g_conf_dir;
 *         cmd += "/my-project";
 *         if(system(cmd.c_str()) != 0)
 *         {
 *             perror(cmd.c_str());
 *             return 1;
 *         }
 *
 *         return 0;
 *     }
 *
 *     }
 *
 *     ...snip...
 *
 *     snap_catch2_main(
 *           "libutf8"
 *         , "1.2.3"
 *         , argc
 *         , argv
 *         , nullptr
 *         , nullptr
 *         , &finish_init
 *     );
 * \endcode
 *
 * Once the tests ran, we call the \p finished_callback as well. This gives
 * you the ability to test futher things such as making sure that everything
 * was cleaned up, resources released, etc. For example, in our advgetopt
 * we use a FIFO to handle error messages. If an error never occurs that
 * FIFO will not be empty on exit. This is actually an error in the test.
 * Only errors that do occur have to be added to the FIFO. So on exit we
 * want to verify that the FIFO is indeed empty.
 *
 * \param[in] project_name  The name of the project (i.e. "libutf8").
 * \param[in] project_version  The version string of the project
 *                     (i.e. "LIBUTF8_VERSION_STRING").
 * \param[in] argc  The number of arguments on the command line.
 * \param[in] argv  An array of strings with the command line arguments.
 * \param[in] init_callback  Callback called before anything else.
 * \param[in] add_user_options  A callback giving you the opportunity to add
 *                              command line options before argv gets parsed.
 * \param[in] callback  A callback called just before we run the tests.
 * \param[in] finished_callback  The callback called just after all the tests
 *                               ran.
 *
 * \return The exit code, usually 0 on success and 1 on an error.
 */
inline int snap_catch2_main(
          char const * project_name
        , char const * project_version
        , int argc
        , char * argv[]
        , void (*init_callback)() = nullptr
        , void (*add_user_options)(Catch::Clara::Parser & cli) = nullptr
        , int (*callback)(Catch::Session & session) = nullptr
        , void (*finished_callback)() = nullptr)
{
    typedef unsigned int seed_t; // see `man srand(3)` for type

    try
    {
        if(init_callback != nullptr)
        {
            init_callback();
        }

        Catch::Session session;

        bool version(false);
        seed_t seed(static_cast<seed_t>(time(NULL)));

        auto cli = session.cli();
        cli |= Catch::Clara::Opt(seed, "seed")
                    ["-S"]["--seed"]
                    ("value to seed the randomizer, if not specified, use time()")
                 | Catch::Clara::Opt(g_progress())
                    ["-p"]["--progress"]
                    ("print name of test section being run")
                 | Catch::Clara::Opt(g_source_dir(), "source_dir")
                    ["--source-dir"]
                    ("specify the full path to the source directory")
                 | Catch::Clara::Opt(g_binary_dir(), "binary_dir")
                    ["--binary-dir"]
                    ("specify the full path to the binary directory")
                 | Catch::Clara::Opt(g_dist_dir(), "dist_dir")
                    ["--dist-dir"]
                    ("specify the full path to the dist directory")
                 | Catch::Clara::Opt(g_tmp_dir(), "tmp_dir")
                    ["-T"]["--tmp-dir"]
                    ("specify a temporary directory")
                 | Catch::Clara::Opt(g_verbose())
                    ["--verbose"]
                    ("print additional information from within our own tests")
                 | Catch::Clara::Opt(version)
                    ["-V"]["--version"]
                    ("print out the libutf8 library version these unit tests pertain to");

        if(add_user_options != nullptr)
        {
            add_user_options(cli);
        }

        session.cli(cli);

        if(session.applyCommandLine(argc, argv) != 0)
        {
            std::cerr << "fatal error: invalid command line." << std::endl;
            return 1;
        }

        if(version)
        {
            std::cout << project_version << std::endl;
            return 0;
        }

        // also turn on verbosity if the VERBOSE environment variable
        // is set to a value other than 0, false, off
        //
        char const * verbose(getenv("VERBOSE"));
        if(verbose != nullptr)
        {
            if(strcmp(verbose, "0") != 0
            && strcmp(verbose, "false") != 0
            && strcmp(verbose, "off") != 0)
            {
                g_verbose() = true;
            }
        }
        if(g_verbose())
        {
            std::cout << "info: verbosity activated." << std::endl;
        }

        detail::init_tmp_dir(project_name);

        // by default we get a different seed each time; that really helps
        // in detecting errors! At least it helped me many times.
        //
        srand(seed);
        srand48(seed);

        // save the seed, it can be practical opposed to searching your
        // test output
        //
        {
            std::ofstream seed_file;
            seed_file.open("seed.txt");
            if(seed_file.is_open())
            {
                seed_file << seed << std::endl;
            }
        }

        if(callback != nullptr)
        {
            int const e(callback(session));
            if(e != 0)
            {
                return e;
            }
        }

        std::cout << project_name
                  << " v"
                  << project_version
                  << " ["
                  << getpid()
                  << "]:unittest: seed is "
                  << seed
                  << "\n"
                     "source directory: \""
                  << g_source_dir()
                  << "\"\n"
                     "binary directory: \""
                  << g_binary_dir()
                  << "\"\n"
                     "dist directory: \""
                  << g_dist_dir()
                  << "\"\n"
                     "temporary directory: \""
                  << g_tmp_dir()
                  << "\""
                  << std::endl;

        auto const r(session.run());

        if(finished_callback != nullptr)
        {
            finished_callback();
        }

        return r;
    }
    catch(std::logic_error const & e)
    {
        std::cerr << "fatal error: caught a logic error in "
                  << project_name
                  << " unit tests: "
                  << e.what()
                  << "\n";
        return 1;
    }
    catch(std::runtime_error const & e)
    {
        std::cerr << "fatal error: caught a run-time error in "
                  << project_name
                  << " unit tests: "
                  << e.what()
                  << "\n";
        return 1;
    }
    catch(std::exception const & e)
    {
        std::cerr << "fatal error: caught a standard exception in "
                  << project_name
                  << " unit tests: "
                  << e.what()
                  << "\n";
        return 1;
    }
}
#endif



inline void catch_compare_long_strings(std::string const & a, std::string const & b)
{
    auto print_char = [](char c)
    {
        if(c == 0x20)
        {
            // Possible characters to make spaces visible are:
            //      U+23B5      BOTTOM SQUARE BRACKET
            //      U+2420      SYMBOL FOR SPACE
            //      U+2423      OPEN BOX
            //
            std::cout << "\xE2\x90\xA3";
        }
        else if(static_cast<unsigned char>(c) < 0x20)
        {
            // control character
            //
            std::cout << '^' << static_cast<char>(c + 0x40);
        }
        else if(static_cast<unsigned char>(c) < 0x80)
        {
            // standard character
            //
            std::cout << c;
        }
        else if(static_cast<unsigned char>(c) < 0xA0)
        {
            // graphical control
            //
            std::cout << '@' << static_cast<char>(c - 0x40);
        }
        else
        {
            // UTF-8 characters cannot be written as is since we are breaking
            // up the string into bytes instead of characters; so instead show
            // the corresponding \xXX value
            //
            std::cout << "\\x" << std::hex << static_cast<int>(static_cast<std::uint8_t>(c));
        }
    };

    if(a != b)
    {
        std::cout << "error: long strings do not match.\n"
                  << "---------------------------------------------------\n";

        // TODO: we may want to look into supporting UTF-8 properly
        //
        std::size_t const max(std::min(a.length(), b.length()));
        bool err(false);
        for(std::size_t idx(0); idx < max; ++idx)
        {
            if(a[idx] == b[idx])
            {
                if(err)
                {
                    err = false;
                    std::cout << "\033[0m";
                }
                std::cout << a[idx];
            }
            else
            {
                if(!err)
                {
                    err = true;
                    std::cout << "\033[7m";
                }
                std::cout << '[';
                print_char(a[idx]);
                std::cout << '/';
                print_char(b[idx]);
                std::cout << ']';
            }
        }
        if(err)
        {
            err = false;
            std::cout << "\033[0m";
        }

        std::cout << std::endl
                  << "---------------------------------------------------" << std::endl;

        if(a.length() > b.length())
        {
            std::cout << "Left hand side string is longer ("
                      << a.length()
                      << " versus "
                      << b.length()
                      << ")."
                      << std::endl;
        }
        else if(b.length() > a.length())
        {
            std::cout << "Right hand side string is longer ("
                      << a.length()
                      << " versus "
                      << b.length()
                      << ")."
                      << std::endl;
        }
    }

    // to generate the standard error too
    //
    CATCH_REQUIRE(a == b);
}


inline void catch_compare_large_buffers(void const * a, std::size_t a_size, void const * b, std::size_t b_size)
{
    if(a_size == b_size
    && memcmp(a, b, a_size) == 0)
    {
        return;
    }

    std::cout << "error: large buffers do not match.\n"
              << "---------------------------------------------------\n";

    std::uint8_t const * a_ptr(reinterpret_cast<std::uint8_t const *>(a));
    std::uint8_t const * b_ptr(reinterpret_cast<std::uint8_t const *>(b));
    std::size_t const max(std::min(a_size, b_size));

    // skip start as long as equal
    //
    std::size_t idx(0);
    for(; idx < max; ++idx)
    {
        if(a_ptr[idx] != b_ptr[idx])
        {
            break;
        }
    }
    if(idx > 10)
    {
        // adjust pointer to include some equal data and then a multiple of 16
        //
        idx -= 10;
        idx &= -16;
    }
    else
    {
        idx = 0;
    }

    // limit the output to 4 lines of data
    //
    std::size_t const limit(std::min(idx + 64, max));

    bool err(false);
    bool first(true);
    std::cout << std::hex << std::setfill('0');
    for(; idx < limit; ++idx)
    {
        if((idx & 15) == 0)
        {
            if(first)
            {
                first = false;
            }
            else
            {
                std::cout << '\n';
            }
            std::cout << "\033[0m" << std::setw(8) << idx << "- ";
            err = false;
        }
        if(a_ptr[idx] == b_ptr[idx])
        {
            if(err)
            {
                err = false;
                std::cout << "\033[0m";
            }
            std::cout << std::setw(2) << a_ptr[idx] << ' ';
        }
        else
        {
            if(!err)
            {
                err = true;
                std::cout << "\033[7m";
            }
            std::cout << '[';
            std::cout << std::setw(2) << a_ptr[idx];
            std::cout << '/';
            std::cout << std::setw(2) << b_ptr[idx];
            std::cout << "] ";
        }
    }
    if(err)
    {
        err = false;
        std::cout << "\033[0m";
    }

    std::cout << (limit != max ? "..." : "") << '\n'
              << "---------------------------------------------------" << std::endl;

    if(a_size > b_size)
    {
        std::cout << "Left hand side buffer is longer ("
                  << a_size
                  << " versus "
                  << b_size
                  << ")."
                  << std::endl;
    }
    else if(b_size > a_size)
    {
        std::cout << "Right hand side buffer is longer ("
                  << a_size
                  << " versus "
                  << b_size
                  << ")."
                  << std::endl;
    }

    // to generate the standard error too
    //
    CATCH_REQUIRE(!"large buffers a and b differ");
}


template<typename F>
F default_epsilon()
{
    return static_cast<F>(0.00001);
}


template<typename F>
bool nearly_equal(
      F const lhs
    , F const rhs
    , F epsilon = default_epsilon<F>())
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    // already equal?
    if(lhs == rhs)
    {
        return true;
    }

    F const diff(std::abs(lhs - rhs));
    if(lhs == 0.0
    || rhs == 0.0
    || diff < std::numeric_limits<F>::min())
    {
        return diff < (epsilon * std::numeric_limits<F>::min());
    }
#pragma GCC diagnostic pop

    return diff / (std::abs(lhs) + std::abs(rhs)) < epsilon;
}





} // SNAP_CATCH2_NAMESPACE namespace


/** \brief Start a new section.
 *
 * This macro is an _overload_ of the CATCH_SECTION() macro defined in
 * catch2.hpp. It is used so we can print out the name of the section
 * whenever it executes and g_progress is true (i.e. the `--progress`
 * command line option was specified.)
 *
 * \code
 *     CATCH_START_SECTION("my section name/description here")
 *         break_something(1, 2, 3);
 *     CATCH_END_SECTION()
 * \endcode
 *
 * As you can see, you want to end the section with the CATCH_END_SECTION()
 * macro.
 *
 * \note
 * You do not need to add a block like you have to do with CATCH since
 * the macros add them for you.
 *
 * \note
 * The older version of Catch supported a description for a section.
 * Now that's useless so we only offer a name here.
 *
 * \param[in] name  The name of the section.
 */
#define CATCH_START_SECTION(name) \
    CATCH_SECTION(name) \
    { \
        if(SNAP_CATCH2_NAMESPACE::g_progress()) \
        { \
            std::cout << "SECTION: " << name << std::endl; \
        }


/** \brief End a section.
 *
 * This macro ends a section. The CATCH_START_SECTION() creates a
 * block which this macro closes.
 */
#define CATCH_END_SECTION() \
    }


/** \brief Require that two long strings be equal.
 *
 * When testing with really long strings, the compare failing will give
 * you both output but it's pretty much impossible to see the errors.
 *
 * Here we call our own function which will highlight the problems
 * so that way you can immediately see where the first difference occurs.
 *
 * \param[in] a  The first string.
 * \param[in] b  The second string.
 */
#define CATCH_REQUIRE_LONG_STRING(a, b) SNAP_CATCH2_NAMESPACE::catch_compare_long_strings(a, b)


/** \brief Require that two long buffers be equal.
 *
 * When testing really large buffers, the compare failing lists all the
 * bytes on each side. By default, Catch will output the complete left
 * hand side and then the complete right hand side. In most cases, that's
 * useless if the buffer is in the kilobytes in size.
 *
 * This macro calls a function which makes sure to render the bytes next
 * to each other and shows the erroneous ones highlighter. It also shows
 * the location inside the buffer as an offset or address.
 *
 * If the first error appears way down, then the part that is equal at
 * the start is skipped. It also only shows the first 64 bytes with the
 * first few errors. In most cases, fixing that part will be sufficient
 * to move forward.
 *
 * \param[in] a  The first buffer.
 * \param[in] a_size  The size of the first buffer.
 * \param[in] b  The second buffer.
 * \param[in] b_size  The size of the second buffer.
 */
#define CATCH_REQUIRE_LARGE_BUFFER(a, a_size, b, b_size) SNAP_CATCH2_NAMESPACE::catch_compare_large_buffers(a, a_size, b, b_size)



/** \brief Compare two floating points for near equality.
 *
 * This macro is a helper which calls our nearly_equal() function with
 * the default epsilon (1e-5).
 *
 * This is useful since comparing floating points with just `==` is
 * often not a good choice if the math on both sides is not exactly
 * the same (and then what are we testing?)
 *
 * \note
 * To still use the equivalent of `==`, you can directly call the
 * nearly_equal() function and set the epsilon to 0.0:
 *
 * \code
 *     SNAP_CATCH2_NAMESPACE::nearly_equal(a, b, 0.0)
 * \endcode
 *
 * \note
 * Catch2 offers an approximation equality which uses a special literal:
 *
 * \code
 *     CATCH_REQUIRE(a == 1.0_a); // equivalent to Approx(1.0)
 * \endcode
 *
 * \param[in] a  The left hand side floating point to compare.
 * \param[in] b  The right hand side floating point to compare.
 */
#define CATCH_REQUIRE_FLOATING_POINT(a, b) SNAP_CATCH2_NAMESPACE::nearly_equal(a, b)



namespace Catch
{
namespace Matchers
{


class ExceptionWatcher
    : public MatcherBase<std::exception>
{
public:
    ExceptionWatcher(std::string const & expected_message, bool verbose)
        : m_expected_message(expected_message)
        , m_verbose(verbose)
    {
    }

    /** \brief Check whether we got a match.
     *
     * This function compares the expected string with the actual exception
     * what() output.
     */
    bool match(std::exception const & e) const override
    {
        if(m_verbose)
        {
            SNAP_CATCH2_NAMESPACE::catch_compare_long_strings(e.what(), m_expected_message);
        }
        return e.what() == m_expected_message;
    }

    /** \brief Describe this matcher.
     *
     * This function produces a string describing what this matcher does.
     *
     * \return The description of this matcher.
     */
    virtual std::string describe() const override
    {
        return "compare the exception what() message with \""
             + m_expected_message
             + "\".";
    }

private:
    std::string     m_expected_message = std::string();
    bool            m_verbose = false;
};


inline ExceptionWatcher ExceptionMessage(std::string const & expected_message, bool verbose = false)
{
    return ExceptionWatcher(expected_message, verbose);
}



}
// Matchers namespace
}
// Catch namespace


// vim: ts=4 sw=4 et
