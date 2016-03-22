//
// Created by qyt on 16/3/22.
//

#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include "FileUtils.hpp"
namespace qyt
{

    static unsigned char* getData(const std::string& filename, bool forString)
    {
        if(filename.empty())
            return nullptr;

        unsigned char* buffer = nullptr;
        size_t size = 0;
        size_t readsize;
        const char* mode = nullptr;

        if(forString)
            mode = "rt";
        else
            mode = "rb";

        auto fileutils = FileUtils::getInstance();
        do
        {
            std::string fullPath = fileutils->fullPathForFilename(filename);
            FILE* fp = fopen(filename.c_str(), mode);
            if(!fp) break;
            fseek(fp, 0, SEEK_END);
            size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            if(forString)
            {
                buffer = (unsigned char*)malloc(sizeof(unsigned char)*(size+1));
                buffer[size] = '\0';
            }
            else
            {
                buffer = (unsigned char*)malloc(sizeof(unsigned char)*(size));
            }
            readsize = fread(buffer, sizeof(unsigned char), size, fp);
            fclose(fp);

            if (forString && readsize < size)
                buffer[readsize] = '\0';
        }while(false);

        if(nullptr == buffer || 0 == readsize)
        {
            std::cerr << "Can not get data from " << filename;
            free(buffer);
            buffer = nullptr;
        }

        return buffer;
    }

    //----------------------------------------------------------------------------
    FileUtils* FileUtils::s_sharedFileUtils = nullptr;


    FileUtils *FileUtils::getInstance()
    {
        if (s_sharedFileUtils == nullptr)
        {
            s_sharedFileUtils = new (std::nothrow) FileUtils();
            if(!s_sharedFileUtils->init())
            {
                delete s_sharedFileUtils;
                s_sharedFileUtils = nullptr;
            }
        }
        return s_sharedFileUtils;
    }


    void FileUtils::distroyInstance()
    {
        delete s_sharedFileUtils;
        s_sharedFileUtils = nullptr;
    }

    FileUtils::FileUtils() {}

    FileUtils::~FileUtils() { }


    std::string FileUtils::getStringFromFile(const std::string &filename)
    {
        unsigned char* buffer = getData(filename, true);
        if(buffer == nullptr)
            return "";
        return std::string((const char*)buffer);
    }


    unsigned char *FileUtils::getDataFromFile(const std::string &filename)
    {
        return getData(filename, false);
    }


    std::string FileUtils::fullPathForFilename(const std::string &filename)
    {
        if(filename.empty())
            return "";

        if(isAbsolutePath(filename))
            return filename;

        auto cacheIter = _fullPathCache.find(filename);
        if(cacheIter != _fullPathCache.end())
            return cacheIter->second;

        std::string fullPath;

        for (const auto &searchIt : _searchPathArray)
        {
            for (const auto& resolutionIt : _searchResolutionsOrderArray)
            {
                fullPath = getPathForFilename(filename, resolutionIt, searchIt);
                if(!fullPath.empty())
                {
                    _fullPathCache.insert(std::make_pair(filename, fullPath));
                    return fullPath;
                }

            }

        }
        return "";
    }

    bool FileUtils::isAbsolutePath(const std::string &path) const {
        return (path[0] == '/');
    }

    std::string FileUtils::getPathForFilename(const std::string &filename,
                                              const std::string &resolutionDirectory,
                                              const std::string &searchPath) const
    {
        std::string file = filename;
        std::string file_path = "";
        size_t pos = file.find_last_of("/");
        if(pos != std::string::npos)
        {
            file_path = filename.substr(0, pos+1);
            file = filename.substr(pos+1);
        }
        std::string path = searchPath;
        path += file_path;
        path += resolutionDirectory;
        path = getFullPathForDirectoryAndFilename(path, file);
        return path;
    }

    std::string FileUtils::getFullPathForDirectoryAndFilename(const std::string &directory,
                                                              const std::string &filename) const
    {
        std::string ret = directory;
        if (directory.size() && directory[directory.size()-1] != '/'){
            ret += '/';
        }
        ret += filename;

        if(!isFileExistInternal(ret))
            ret = "";
        return ret;

    }

    bool FileUtils::init()
    {
        _defaultResRootPath = "../../resource";
        _searchPathArray.push_back(_defaultResRootPath);
        _searchResolutionsOrderArray.push_back("");
        return true;
    }

    void FileUtils::setSearchResolutionsOrder(const std::vector<std::string> &searchResolutionsOrder)
    {
        bool existDefault = false;
        _fullPathCache.clear();
        _searchResolutionsOrderArray.clear();
        for(const auto& iter : searchResolutionsOrder)
        {
            std::string resolutionDirectory = iter;
            if (!existDefault && resolutionDirectory == "")
            {
                existDefault = true;
            }

            if (resolutionDirectory.length() > 0 && resolutionDirectory[resolutionDirectory.length()-1] != '/')
            {
                resolutionDirectory += "/";
            }

            _searchResolutionsOrderArray.push_back(resolutionDirectory);
        }

        if (!existDefault)
        {
            _searchResolutionsOrderArray.push_back("");
        }
    }

    void FileUtils::addSearchResolutionsOrder(const std::string &order,const bool front)
    {
        std::string resOrder = order;
        if (!resOrder.empty() && resOrder[resOrder.length()-1] != '/')
            resOrder.append("/");

        if (front) {
            _searchResolutionsOrderArray.insert(_searchResolutionsOrderArray.begin(), resOrder);
        } else {
            _searchResolutionsOrderArray.push_back(resOrder);
        }
    }

    const std::vector<std::string> &FileUtils::getSearchResolutionsOrder() const {
        return _searchResolutionsOrderArray;
    }

    void FileUtils::setSearchPaths(const std::vector<std::string>& searchPaths)
    {
        bool existDefaultRootPath = false;

        _fullPathCache.clear();
        _searchPathArray.clear();
        for (const auto& iter : searchPaths)
        {
            std::string prefix;
            std::string path;

            if (!isAbsolutePath(iter))
            { // Not an absolute path
                prefix = _defaultResRootPath;
            }
            path = prefix + (iter);
            if (!path.empty() && path[path.length()-1] != '/')
            {
                path += "/";
            }
            if (!existDefaultRootPath && path == _defaultResRootPath)
            {
                existDefaultRootPath = true;
            }
            _searchPathArray.push_back(path);
        }

        if (!existDefaultRootPath)
        {
            _searchPathArray.push_back(_defaultResRootPath);
        }
    }

    void FileUtils::addSearchPath(const std::string &searchpath,const bool front)
    {
        std::string prefix;
        if (!isAbsolutePath(searchpath))
            prefix = _defaultResRootPath;

        std::string path = prefix + searchpath;
        if (!path.empty() && path[path.length()-1] != '/')
        {
            path += "/";
        }
        if (front) {
            _searchPathArray.insert(_searchPathArray.begin(), path);
        } else {
            _searchPathArray.push_back(path);
        }
    }

    void FileUtils::setDefaultResourceRootPath(const std::string& path)
    {
        _defaultResRootPath = path;
    }

    const std::vector<std::string>& FileUtils::getSearchPaths() const
    {
        return _searchPathArray;
    }

    std::string FileUtils::getFileExtension(const std::string& filePath) const
    {
        std::string fileExtension;
        size_t pos = filePath.find_last_of('.');
        if (pos != std::string::npos)
        {
            fileExtension = filePath.substr(pos, filePath.length());

            std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
        }

        return fileExtension;
    }

    long FileUtils::getFileSize(const std::string &filepath)
    {
        if(filepath.empty())
            return 0;

        std::string fullpath = filepath;
        if (!isAbsolutePath(filepath))
        {
            fullpath = fullPathForFilename(filepath);
            if (fullpath.empty())
                return 0;
        }

        struct stat info;
        int result = stat(fullpath.c_str(), &info);

        if (result != 0)
            return -1;
        else
            return (long)(info.st_size);

    }

    void FileUtils::purgeCachedEntries()
    {
        _fullPathCache.clear();
    }


    bool FileUtils::isFileExistInternal(const std::string& filePath) const
    {
        if (filePath.empty())
        {
            return false;
        }

        bool ret = false;
        int accret = -1;

        if (filePath[0] != '/')
        {
            std::string path;
            std::string file;
            size_t pos = filePath.find_last_of("/");
            if (pos != std::string::npos) {
                file = filePath.substr(pos + 1);
                path = filePath.substr(0, pos + 1);
            }
            else {
                file = filePath;
            }

            std::string fullpath = "";
            if (filePath[0] != '.')
                fullpath = _defaultResRootPath+filePath;
            else
                fullpath = filePath;

                accret = access(fullpath.c_str(), F_OK);
            if(accret != -1)
                ret = true;
            else
            {
                ret = false;
            }
        }
        else
        {
            accret = access(filePath.c_str(), F_OK);
            if(accret != -1)
                ret = true;
            else
            {
                ret = false;
            }
        }
        return ret;
    }
}