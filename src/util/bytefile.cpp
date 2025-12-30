#include "util/bytefile.hpp"

#include <plog/Log.h>
#include <fmt/format.h>

namespace top1 {

  /****************************************/
  /* ByteFile Implementation              */
  /****************************************/

  ByteFile::ByteFile() {}

  ByteFile::ByteFile(const Path& p) {
    open(p);
  }

  ByteFile::ByteFile(ByteFile&&) {
    // TODO: Do moves
  }

  ByteFile::~ByteFile() {
    if (is_open()) {
      close();
    }
  }

  ByteFile& ByteFile::operator=(ByteFile other) {
    swap(*this, other);
    return *this;
  }

  void swap(ByteFile& first, ByteFile& sec) {
    using std::swap;
    // TODO: Do swaps;
  }

  void ByteFile::open(const Path& p) {
    if (is_open()) {
      throw "File already open";
    }
    path = p;
    fstream.open(p, std::ios::in | std::ios::out | std::ios::binary);
    if (!fstream) {
      // File didnt exist, create it
      create_file();
    }
    read_file();
  }

  void ByteFile::close() {
    if (fstream.is_open()) {
      write_file();
      fstream.close();
    };
  }

  void ByteFile::flush() {
    if (fstream.is_open()) {
      write_file();
      fstream.flush();
    };
  }

  void ByteFile::create_file() {
    // Close the fstream without calling write_file() (which would fail on empty file)
    if (fstream.is_open()) {
      fstream.close();
    }
    fstream.open(path, std::ios::trunc | std::ios::out | std::ios::binary);
    fstream.close();
    fstream.open(path, std::ios::in | std::ios::out | std::ios::binary);
    write_file();
  }

  void ByteFile::read_file() {
    seek(0);
  }
  void ByteFile::write_file() {
    seek(0);
  }

  bool ByteFile::is_open() const {
    return fstream.is_open();
  }

  ByteFile::Position ByteFile::seek(Position p, std::ios::seekdir d) {
    if (!is_open()) throw Error(Error::Type::FileNotOpen, "ByteFile::seek(p, d)");
    if (p < 0 && d == std::ios::beg) {
      p = 0;
    }
    fstream.seekg(p, d);
    fstream.seekp(p, d);
    return p;
  }

  ByteFile::Position ByteFile::position() {
    if (!is_open()) throw Error(Error::Type::FileNotOpen, "ByteFile::position()");
    auto pos = fstream.tellg();
    LOGD << pos;
    if (pos < 0) {
      // Stream is in a bad state, try to recover
      fstream.clear();
      fstream.seekg(0, std::ios::beg);
      fstream.seekp(0, std::ios::beg);
      return 0;
    }
    auto r = fstream.seekp(pos).tellp();
    if (fstream.eof()) {
      fstream.clear();
      throw Error(Error::Type::PastEnd);
    }
    if (!fstream.good()) {
      fstream.clear();
      throw Error(Error::Type::ExceptionThrown,
        std::strerror(errno));
    }
    if (r < 0) {
      LOGE << "got negative position from file";
    }
    return r;
  }

  ByteFile::Position ByteFile::size() {
    if (!is_open()) throw Error(Error::Type::FileNotOpen, "ByteFile::size()");
    auto p = fstream.tellg();
    auto end = fstream.seekg(0, std::fstream::end).tellg();
    fstream.seekg(p);
    return Position(end);
  }

} // top1
