#ifndef LIBFPLIB_FPLIB_VERSION_H_9ABE03B1_
#define LIBFPLIB_FPLIB_VERSION_H_9ABE03B1_

#define LIBFPLIB_NAME_STR "libfplib"
#define LIBFPLIB_VERSION_STR "1.6.1"
#define LIBFPLIB_DESCRIPTION_STR "Last FM fingerprint library runtime"
#define LIBFPLIB_COPYRIGHT_STR "(C) 2012 Last.fm Ltd"
#define LIBFPLIB_COPYRIGHT_FULL_STR "Copyright (C) 2012 Last.fm Ltd"

#define LIBFPLIB_PROJECT_STR "libfplib"
#define LIBFPLIB_PACKAGE_STR "lastfm-libfplib"
#define LIBFPLIB_AUTHOR_STR "Ricky Cormier <ricky@last.fm>"
#define LIBFPLIB_RELEASE_ISODATE_STR "2012-05-23T15:17:49+00:00"
#define LIBFPLIB_RELEASE_YEAR_STR "2012"
#define LIBFPLIB_RELEASE_DATE_STR "2012-05-23"
#define LIBFPLIB_RELEASE_TIME_STR "15:17:49"
#define LIBFPLIB_FULL_REVISION_STR "1.6.1"
#define LIBFPLIB_REVISION_STR "1.6.1"

#define LIBFPLIB_RELEASE_YEAR 2012
#define LIBFPLIB_RELEASE_EPOCH_TIME 1337786269
#define LIBFPLIB_MAJOR_REVISION 1
#define LIBFPLIB_MINOR_REVISION 6
#define LIBFPLIB_PATCHLEVEL 1

#ifdef __cplusplus

#include <string>

namespace fm { namespace last { namespace version {

   ////////////////////////////////////////////////////////////////////////////
   // Use this class if you just need access to version info and have no need
   // to instantiate an object that will provide access to that version info.

   struct libfplib_vinfo
   {
      static std::string name() { return LIBFPLIB_NAME_STR; }
      static std::string version() { return LIBFPLIB_VERSION_STR; }
      static std::string description() { return LIBFPLIB_DESCRIPTION_STR; }
      static std::string copyright() { return LIBFPLIB_COPYRIGHT_STR; }
      static std::string copyright_full() { return LIBFPLIB_COPYRIGHT_FULL_STR; }
      static std::string project_str() { return LIBFPLIB_PROJECT_STR; }
      static std::string package_str() { return LIBFPLIB_PACKAGE_STR; }
      static std::string author_str() { return LIBFPLIB_AUTHOR_STR; }
      static std::string release_isodate_str() { return LIBFPLIB_RELEASE_ISODATE_STR; }
      static std::string release_year_str() { return LIBFPLIB_RELEASE_YEAR_STR; }
      static std::string release_date_str() { return LIBFPLIB_RELEASE_DATE_STR; }
      static std::string release_time_str() { return LIBFPLIB_RELEASE_TIME_STR; }
      static std::string full_revision_str() { return LIBFPLIB_FULL_REVISION_STR; }
      static std::string revision_str() { return LIBFPLIB_REVISION_STR; }
      static int release_year() { return LIBFPLIB_RELEASE_YEAR; }
      static time_t release_epoch_time() { return LIBFPLIB_RELEASE_EPOCH_TIME; }
      static int major_revision() { return LIBFPLIB_MAJOR_REVISION; }
      static int minor_revision() { return LIBFPLIB_MINOR_REVISION; }
      static int patchlevel() { return LIBFPLIB_PATCHLEVEL; }

   };

   ////////////////////////////////////////////////////////////////////////////
   // Use this class if you have an existing interface that defines the vinfo
   // functions and expects them to be implemented by a derived class.

   template <typename Base>
   struct libfplib_vinfo_ : Base
   {
      virtual std::string name() const { return libfplib_vinfo::name(); }
      virtual std::string version() const { return libfplib_vinfo::version(); }
      virtual std::string description() const { return libfplib_vinfo::description(); }
      virtual std::string copyright() const { return libfplib_vinfo::copyright(); }
      virtual std::string copyright_full() const { return libfplib_vinfo::copyright_full(); }
      virtual std::string project_str() const { return libfplib_vinfo::project_str(); }
      virtual std::string package_str() const { return libfplib_vinfo::package_str(); }
      virtual std::string author_str() const { return libfplib_vinfo::author_str(); }
      virtual std::string release_isodate_str() const { return libfplib_vinfo::release_isodate_str(); }
      virtual std::string release_year_str() const { return libfplib_vinfo::release_year_str(); }
      virtual std::string release_date_str() const { return libfplib_vinfo::release_date_str(); }
      virtual std::string release_time_str() const { return libfplib_vinfo::release_time_str(); }
      virtual std::string full_revision_str() const { return libfplib_vinfo::full_revision_str(); }
      virtual std::string revision_str() const { return libfplib_vinfo::revision_str(); }
      virtual int release_year() const { return libfplib_vinfo::release_year(); }
      virtual time_t release_epoch_time() const { return libfplib_vinfo::release_epoch_time(); }
      virtual int major_revision() const { return libfplib_vinfo::major_revision(); }
      virtual int minor_revision() const { return libfplib_vinfo::minor_revision(); }
      virtual int patchlevel() const { return libfplib_vinfo::patchlevel(); }

   };

   ////////////////////////////////////////////////////////////////////////////

} } }

#endif /* __cplusplus */

#endif /* LIBFPLIB_FPLIB_VERSION_H_9ABE03B1_ */
