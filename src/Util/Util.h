/* Copyright 2017 - BSD-3-Clause
 *
 * Copyright Holder (alphabetical):
 *
 * Beikirch, Maximilian
 * Cramer, Simon
 * Frank, Martin
 * Otte, Philipp
 * Pabich, Emma
 * Trimborn, Torsten
 *
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *    disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *    following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * @author Beikirch, Cramer, Pabich
 * @date 08 Nov 2017
 * @brief This file belongs to the SABCEMM projekt. See github.com/SABCEMM/SABCEMM
 */

#ifndef UTIL_H_
#define UTIL_H_

#if defined(__APPLE__) || defined(__unix__)
   #include <sys/types.h>
   #include <sys/stat.h>
#elif defined(__WIN32__) || defined(_MS_DOS_)
    #include <dir.h>
#else
    #include <direct.h>  /* Visual C++ */
#endif
#include <cstddef> //for std::size_t
#include <string>
#include <vector>
#include <numeric>
#include <cmath>
#include <limits>
#include <algorithm>


class Util
{
public:

    /**
     * @brief platform-independent routine that creates a folder.
     *        Will fail silently if folder exists.
     * @todo  maybe add the functionality to recursively create chains of
     *        of folders? Or use boost for that?
     */
    static void createDirectory(const std::string &folder)
    {
        // default linux permission for folders (0775)
        #if defined(__APPLE__) || defined(__unix__)
            mkdir(folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        #else
            mkdir(folder.c_str());
        #endif
    }

    static double mean(std::vector<double>& v){
        double sum = std::accumulate(v.begin(), v.end(), 0.0);
        double mean = sum / static_cast<double>(v.size());
        return mean;
    }
    static double std(std::vector<double>& v){
        double mean_ = mean(v);
        std::vector<double> diff(v.size());
        std::transform(v.begin(), v.end(), diff.begin(), [mean_](double x) { return x - mean_; });
        double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
        double var = std::sqrt(sq_sum / static_cast<double>(v.size()));
        return var;
    }

    static std::size_t getMaxDimensions(std::vector<std::vector<double>> *v){
        std::vector<double>::size_type maxDim = 0;
        for (auto &i : *v) {
            if(i.size()>maxDim){
                maxDim = i.size();
            }
        }
        return maxDim;
    }

    enum DataItemCollectorMethod
    {
        MEAN,
        STD,
        DETAIL
    };

    static std::string dataItemCollectorMethodToString(DataItemCollectorMethod method){
        std::string methodString;
        if(method == Util::DataItemCollectorMethod::STD){
            methodString = "std";
        }
        else if(method == Util::DataItemCollectorMethod::DETAIL){
            methodString = "detail";
        }
        else { // Default method is mean
            methodString = "mean";
        }
        return methodString;
    }

    static DataItemCollectorMethod stringToDataItemCollectorMethod(const std::string &methodString){
        Util::DataItemCollectorMethod method;
        if(methodString == std::string("std")){
             method = Util::DataItemCollectorMethod::STD;
        }
        else if(methodString == std::string("detail")){
            method = Util::DataItemCollectorMethod::DETAIL;
        }
        else { // Default method is mean
            method = Util::DataItemCollectorMethod::MEAN;
        }
        return method;
    }

    static bool doubleEqual(const double& d1, const double& d2){
        return std::abs(d1-d2)<std::numeric_limits<double>::epsilon();
    }

    static bool doubleNotEqual(const double& d1, const double& d2){
        return std::abs(d1-d2)>std::numeric_limits<double>::epsilon();
    }

};

#endif
