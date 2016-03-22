//
// Created by qyt on 16/3/22.
//

#ifndef OPENGL_FILEUTILS_HPP
#define OPENGL_FILEUTILS_HPP

#include <string>
#include <vector>
#include <unordered_map>

namespace qyt
{
    class FileUtils
    {
    public:
        static FileUtils* getInstance();
        static void distroyInstance();

        virtual ~FileUtils();
        virtual std::string getStringFromFile(const std::string& filename);
        virtual unsigned char* getDataFromFile(const std::string& filename);
        virtual std::string fullPathForFilename(const std::string& filename);
        virtual void setSearchResolutionsOrder(const std::vector<std::string>& searchResolutionsOrder);
        virtual void addSearchResolutionsOrder(const std::string &order,const bool front=false);
        virtual const std::vector<std::string>& getSearchResolutionsOrder() const;
        virtual void setSearchPaths(const std::vector<std::string>& searchPaths);
        void setDefaultResourceRootPath(const std::string& path);
        void addSearchPath(const std::string & path, const bool front=false);
        virtual const std::vector<std::string>& getSearchPaths() const;
        virtual std::string getFileExtension(const std::string& filePath) const;
        virtual bool isAbsolutePath(const std::string& path) const;
        virtual long getFileSize(const std::string &filepath);
        virtual void purgeCachedEntries();

    protected:
        FileUtils();
        virtual bool isFileExistInternal(const std::string& filePath) const;
        virtual bool init();
        virtual std::string getPathForFilename(const std::string& filename, const std::string& resolutionDirectory, const std::string& searchPath) const;
        virtual std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const;
        std::vector<std::string> _searchResolutionsOrderArray;
        std::vector<std::string> _searchPathArray;
        std::string _defaultResRootPath;
        mutable std::unordered_map<std::string, std::string> _fullPathCache;

        static FileUtils* s_sharedFileUtils;
    };
}

#endif //OPENGL_FILEUTILS_HPP
