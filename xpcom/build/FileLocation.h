/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_FileLocation_h
#define mozilla_FileLocation_h

#include "nsString.h"
#include "nsCOMPtr.h"
#include "nsAutoPtr.h"
#include "nsIFile.h"
#include "FileUtils.h"

class nsZipArchive;
class nsZipItem;

namespace mozilla {

class FileLocation
{
public:
  /**
   * FileLocation is an helper to handle different kind of file locations
   * within Gecko:
   * - on filesystems
   * - in archives
   * - in archives within archives
   * As such, it stores a path within an archive, as well as the archive
   * path itself, or the complete file path alone when on a filesystem.
   * When the archive is in an archive, an nsZipArchive is stored instead
   * of a file path.
   */
  FileLocation();
  ~FileLocation();

  /**
   * Constructor for plain files
   */
  explicit FileLocation(nsIFile* aFile);

  /**
   * Constructors for path within an archive. The archive can be given either
   * as nsIFile or nsZipArchive.
   */
  FileLocation(nsIFile* aZip, const char* aPath);

  FileLocation(nsZipArchive* aZip, const char* aPath);

  /**
   * Creates a new file location relative to another one.
   */
  FileLocation(const FileLocation& aFile, const char* aPath = nullptr);

  /**
   * Initialization functions corresponding to constructors
   */
  void Init(nsIFile* aFile);

  void Init(nsIFile* aZip, const char* aPath);

  void Init(nsZipArchive* aZip, const char* aPath);

  /**
   * Returns an URI string corresponding to the file location
   */
  void GetURIString(nsACString& aResult) const;

  /**
   * Returns the base file of the location, where base file is defined as:
   * - The file itself when the location is on a filesystem
   * - The archive file when the location is in an archive
   * - The outer archive file when the location is in an archive in an archive
   */
  already_AddRefed<nsIFile> GetBaseFile();

  /**
   * Returns whether the "base file" (see GetBaseFile) is an archive
   */
  bool IsZip() const { return !mPath.IsEmpty(); }

  /**
   * Returns the path within the archive, when within an archive
   */
  void GetPath(nsACString& aResult) const { aResult = mPath; }

  /**
   * Boolean value corresponding to whether the file location is initialized
   * or not.
   */
#if defined(MOZILLA_XPCOMRT_API)
  operator bool() const { return mBaseFile; }
#else
  operator bool() const { return mBaseFile || mBaseZip; }
#endif // defined(MOZILLA_XPCOMRT_API)

  /**
   * Returns whether another FileLocation points to the same resource
   */
  bool Equals(const FileLocation& aFile) const;

  /**
   * Data associated with a FileLocation.
   */
  class Data
  {
  public:
    /**
     * Returns the data size
     */
    nsresult GetSize(uint32_t* aResult);

    /**
     * Copies the data in the given buffer
     */
    nsresult Copy(char* aBuf, uint32_t aLen);
  protected:
    friend class FileLocation;
#if !defined(MOZILLA_XPCOMRT_API)
    nsZipItem* mItem;
#endif // !defined(MOZILLA_XPCOMRT_API)
    nsRefPtr<nsZipArchive> mZip;
    mozilla::AutoFDClose mFd;
  };

  /**
   * Returns the data associated with the resource pointed at by the file
   * location.
   */
  nsresult GetData(Data& aData);
private:
  nsCOMPtr<nsIFile> mBaseFile;
#if !defined(MOZILLA_XPCOMRT_API)
  nsRefPtr<nsZipArchive> mBaseZip;
#endif // !defined(MOZILLA_XPCOMRT_API)
  nsCString mPath;
}; /* class FileLocation */

} /* namespace mozilla */

#endif /* mozilla_FileLocation_h */
