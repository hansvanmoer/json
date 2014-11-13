/* 
 * File:   JSONInput.h
 * Author: hans
 *
 * Created on 31 October 2014, 19:03
 */

#ifndef JSONINPUT_H
#define	JSONINPUT_H

#include <string>
#include <iostream>

namespace JSON {

    template<typename Char_, typename CharTraits_> class EagerInputBuffer {
    public:
        using Char = Char_;
        using CharTraits = CharTraits_;
        using InputStream = std::basic_istream<Char, CharTraits>;

        class Range {
        private:
            Char *begin_;
            Char *end_;
        public:

            Range() : begin_(), end_() {
            };

            Range(Char *begin, Char *end) : begin_(begin), end_(end) {
            };

            Char &operator*() const {
                return *begin_;
            };

            operator bool() const {
                return begin_ != end_;
            };

            bool operator!() const {
                return begin_ == end_;
            };

            Range &operator++() {
                ++begin_;
                return *this;
            };

            Range operator++(int) {
                Range r = *this;
                ++begin_;
                return r;
            };

            bool operator==(const Range &range) const {
                return begin_ == range.begin_;
            };

            bool operator!=(const Range &range) const {
                return begin_ != range.begin_;
            };
        };

    private:

        InputStream &input_;
        Char *buffer_;
        std::streamsize length_;

        void fill() {
            std::streampos cur = input_.tellg();
            input_.seekg(0, std::ios_base::end);
            std::streampos end = input_.tellg();
            length_ = static_cast<std::streamsize> (end - cur);
            buffer_ = new Char[length_];
            input_.seekg(cur, std::ios_base::beg);
            input_.read(buffer_, length_);
            length_ = static_cast<std::streamsize> (input_.gcount());
        };

    public:

        EagerInputBuffer(InputStream &&input) : input_(input), buffer_(), length_() {
        };

        EagerInputBuffer(InputStream &input) : input_(input), buffer_(), length_() {
        };

        ~EagerInputBuffer() {
            if (buffer_) {
                delete[] buffer_;
            }
        };

        Range read() {
            fill();
            return Range{buffer_, buffer_ + length_};
        };
    };

    template<typename Char, typename CharTraits> class LazyInputBuffer {
    public:
        using InputStream = std::basic_istream<Char, CharTraits>;

        class Range;

        friend class Range;
    private:

        struct Block {
        private:
            unsigned int locks_;
        public:
            Char *begin;
            Char *end;
            Block *next;

            Block(Char *data, std::streamsize length) : begin(data), end(data + length), next(), locks_(0) {
            };

            ~Block() {
                delete[] begin;
            };

            void acquire() {
                ++locks_;
            };

            bool release() {
                return (--locks_) == 0;
            };

            bool free() {
                return locks_ == 0;
            };

            bool locked() {
                return locks_ != 0;
            };

            Block(const Block &) = delete;
            Block &operator=(const Block &) = delete;
        };

        InputStream &input_;
        std::size_t blockLength_;
        Block *firstBlock_;

        Block *createBlock() {
            if (input_.good()) {
                Char *data = new Char[blockLength_];
                input_.read(data, blockLength_);
                std::streamsize length = input_.gcount();
                if (length == 0) {
                    delete[] data;
                    return nullptr;
                } else {
                    return new Block(data, length);
                }
            } else {
                return nullptr;
            }
        };

        void checkBlocks() {
            while (firstBlock_ && firstBlock_->free()) {
                Block *old = firstBlock_;
                firstBlock_ = firstBlock_->next;
                delete old;
            }
        };

        Block *getNextBlock(Block *current) {
            Block *next = current->next;
            if (next) {
                next->acquire();
            } else {
                current->next = next = createBlock();
                if (next) {
                    next->acquire();
                }
            }
            current->release();
            checkBlocks();
            return next;
        };

    public:

        class Range {
        private:

            LazyInputBuffer<Char, CharTraits> *buffer_;
            Block *block_;
            Char *current_;

        public:

            Range() : buffer_(), block_(), current_() {
            };

            Range(LazyInputBuffer<Char, CharTraits> *buffer, Block *block) : buffer_(buffer), block_(block), current_(block_->begin) {
                block_->acquire();
            };

            Range(const Range &i) : buffer_(i.buffer_), block_(i.block_), current_(i.current_) {
                if (block_) {
                    block_->acquire();
                }
            };

            Range(Range &&i) : buffer_(i.buffer_), block_(i.block_), current_(i.current_) {
                i.buffer_ = nullptr;
                i.block_ = nullptr;
                i.current_ = nullptr;
            };

            Char *current() const {
                return current_;
            };

            Char *end() const {
                return nullptr;
            };

            Char &operator*() const {
                return *current_;
            };

            bool operator==(const Range &i) const {
                return current_ == i.current_;
            };

            bool operator!=(const Range &i) const {
                return current_ != i.current_;
            };

            bool operator!() const {
                return !current_;
            };

            operator bool() const {
                return current_;
            };

            Range &operator++() {
                ++current_;
                if (current_ == block_->end) {
                    block_ = buffer_->getNextBlock(block_);
                    if (block_) {
                        current_ = block_->begin;
                    } else {
                        current_ = nullptr;
                    }
                }
                return *this;
            };

            Range operator++(int) {
                Range range = *this;
                operator++();
                return range;
            };

            Range &operator=(const Range &i) {
                if (i != this) {
                    if (block_) {
                        block_->release();
                        buffer_->checkBlocks();
                    }
                    buffer_ = i.buffer_;
                    block_ = i.block_;
                    current_ = i.current_;
                    if (block_) {
                        block_->acquire();
                    }
                }
                return *this;
            };

            Range &operator=(Range &&i) {
                std::swap(buffer_, i.buffer_);
                std::swap(block_, i.block_);
                std::swap(current_, i.current_);
                return *this;
            };

            ~Range() {
                if (block_) {
                    block_->release();
                    buffer_->checkBlocks();
                }
            };

        };

        LazyInputBuffer(InputStream &input, std::streamsize blockLength) : input_(input), blockLength_(blockLength), firstBlock_() {
        };

        LazyInputBuffer(InputStream &&input, std::streamsize blockLength) : input_(input), blockLength_(blockLength), firstBlock_() {
        };

        LazyInputBuffer(const LazyInputBuffer<Char, CharTraits> &) = delete;

        LazyInputBuffer<Char, CharTraits> &operator=(const LazyInputBuffer<Char, CharTraits> &) = delete;

        ~LazyInputBuffer() {
            while (firstBlock_) {
                Block *old = firstBlock_;
                firstBlock_ = firstBlock_->next;
                delete old;
            }
        };

        Range read() {
            if (firstBlock_) {
                return Range{this, firstBlock_};
            } else {
                firstBlock_ = createBlock();
                if (firstBlock_) {
                    return Range{this, firstBlock_};
                } else {
                    return Range{};
                }
            }
        };

    };

}


#endif	/* JSONINPUT_H */

