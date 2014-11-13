/* 
 * File:   JSONStoragePolicy.h
 * Author: hans
 *
 * Created on 24 October 2014, 19:55
 */

#ifndef JSONSTORAGEPOLICY_H
#define	JSONSTORAGEPOLICY_H

#include "JSON.h"
#include "JSONException.h"

#include <string>
#include <list>

namespace JSON {

    class NodeTypeException : public JSONException {
    private:
        NodeType expected_;
        NodeType real_;
    public:
        NodeTypeException(NodeType expected, NodeType real);

        NodeType expectedType() const;

        NodeType nodeType() const;
    };

    template<typename JSONTraits> class StrictStoragePolicy {
    private:

        class Data {
        private:

            NodeType type_;

        protected:

            Data(NodeType type) : type_(type) {};
        public:

            virtual typename JSONTraits::Number number() const {
                throw NodeTypeException(NodeType::NUMBER, type_);
            };

            virtual typename JSONTraits::Boolean boolean() const {
                throw NodeTypeException(NodeType::BOOLEAN, type_);
            };

            virtual typename JSONTraits::String string() const {
                throw NodeTypeException(NodeType::STRING, type_);
            };

            virtual void null() const {
                throw NodeTypeException(NodeType::NULL_VALUE, type_);
            };

            virtual typename JSONTraits::NodeId objectId() const {
                throw NodeTypeException(NodeType::OBJECT, type_);
            };

            virtual std::list<Data*> &arrayElements() {
                throw NodeTypeException(NodeType::ARRAY, type_);
            };

            virtual const std::list<Data*> &arrayElements() const {
                throw NodeTypeException(NodeType::ARRAY, type_);
            };

            NodeType getType() const {
                return type_;
            };
        };
        
        class NumberData : public Data {
        private:
            typename JSONTraits::Number value_;

        public:

            NumberData(typename JSONTraits::Number value) : Data(NodeType::NUMBER), value_(value) {
            };

            typename JSONTraits::Number number() const {
                return value_;
            };
        };

        class BooleanData : public Data {
        private:
            typename JSONTraits::Boolean value_;

        public:

            BooleanData(typename JSONTraits::Boolean value) : Data(NodeType::BOOLEAN), value_(value) {
            };

            typename JSONTraits::Boolean boolean() const {
                return value_;
            };
        };

        class StringData : public Data {
        private:
            typename JSONTraits::String value_;

        public:

            StringData(typename JSONTraits::String value) : Data(NodeType::STRING), value_(value) {
            };

            typename JSONTraits::String string() const {
                return value_;
            };
        };

        class NullData : public Data {
        public:

            NullData() : Data(NodeType::NULL_VALUE) {
            };

            void null() const {
            };

        };

        class ObjectData : public Data {
        private:
            typename JSONTraits::NodeId value_;
        public:

            ObjectData(typename JSONTraits::NodeId value) : Data(NodeType::OBJECT), value_(value) {
            };

            typename JSONTraits::NodeId objectId() const {
                return value_;
            };

        };

        class ArrayData : public Data {
        private:
            std::list<Data*> value_;

        public:

            ArrayData() : Data(NodeType::ARRAY), value_() {
            };

            std::list<Data*> &arrayElements() {
                return value_;
            };

            const std::list<Data*> &arrayElements() const {
                return value_;
            };
        };


    public:

        using NodeData = Data *;
        
        static NodeData defaultNode(){
            return nullptr;
        };

        static void destroyNode(NodeData node){
            delete node;
        };
        
        static NodeData createObject(typename JSONTraits::NodeId id) {
            return new ObjectData{id};
        };

        static NodeData createArray() {
            return new ArrayData{};
        };

        static NodeData createString(typename JSONTraits::String string) {
            return new StringData{string};
        };

        static NodeData createNumber(typename JSONTraits::Number number) {
            return new NumberData{number};
        };

        static NodeData createBoolean(typename JSONTraits::Boolean boolean) {
            return new BooleanData{boolean};
        };

        static NodeData createNull() {
            return new NullData{};
        };

        static typename JSONTraits::Number number(NodeData node) {
            return node->number();
        };

        static typename JSONTraits::Boolean boolean(NodeData node) {
            return node->boolean();
        };

        static typename JSONTraits::String string(NodeData node) {
            return node->string();
        };

        static void null(NodeData node) {
            return node->null();
        };

        static typename JSONTraits::NodeId objectId(NodeData node) {
            return node->objectId();
        };

        static std::list<NodeData> &arrayElements(NodeData node) {
            return node->arrayElements();
        };
    };
    
}

#endif	/* JSONSTORAGEPOLICY_H */

