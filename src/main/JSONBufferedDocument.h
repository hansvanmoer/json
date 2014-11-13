#ifndef JSON_BUFFERED_DOCUMENT_H
#define JSON_BUFFERED_DOCUMENT_H

#include <list>

#include "JSON.h"

namespace JSON {

    template<
    typename JSONTraits = BasicJSONTraits,
    typename TypePolicy = BasicTypePolicy<JSONTraits>,
    typename StoragePolicy = BasicStoragePolicy<JSONTraits>
    > class BufferedDocument : private TypePolicy, private StoragePolicy {
    private:

        using Document = BufferedDocument<JSONTraits, TypePolicy, StoragePolicy>;

        using NodeData = typename StoragePolicy::NodeData;

    public:

        using String = typename TypePolicy::String;
        using Number = typename TypePolicy::Number;
        using Boolean = typename TypePolicy::Boolean;
        using NodeId = unsigned long;

    private:

        struct FieldId {
            NodeId parentId;
            String fieldName;

            FieldId() : parentId(), fieldName() {
            };

            FieldId(NodeId parentId_, String fieldName_) : parentId(parentId_), fieldName(fieldName_) {
            };
        };

        std::unordered_map<FieldId, NodeData> fields_;
        NodeData empty_;

        NodeData getNodeData(FieldId field) const {
            auto found = fields_.find(field);
            if (found == fields_.end()) {
                throw NoSuchFieldException<String>(field.fieldName());
            } else {
                return found->second;
            }
        };

        NodeData findNodeData(FieldId field) const {
            auto found = fields_.find(field);
            if (found == fields_.end()) {
                return empty_;
            } else {
                return found->second;
            }
        };

    public:

        class Node;

        friend class Node;

        class Array;

        class Object;

        class NodeBase {
        protected:
            const Document *document_;
            NodeData data_;

            NodeBase(const Document *document, NodeData data) : document_(document), data_(data) {
            };

        public:

            bool operator!() const {
                return !data_;
            };

            operator bool() const {
                return data_;
            };

            NodeType type() const {
                return document->type(data_);
            };
        };

        class Node : public NodeBase {
        public:

            Node(const Document *document, NodeData data) : NodeBase(document, data) {

            };

            String string() const {
                return document_->toString(data_.string());
            };

            Number number() const {
                return document_->toNumber(data_.number());
            };

            Boolean boolean() const {
                return document_->toBoolean(data_.boolean());
            };

            NodeType type() const {
                return data_->type();
            };

            bool null() const {
                return data_.null();
            };

            Object object() const{
                return Object{document_, data_};
            };

            Array array() const{
                return Array{document_, data_};
            };

        };

        class Object : public NodeBase {
        private:
            NodeId id_;
        public:

            Object(const Document *document, NodeData data) : NodeBase(document, data), id_(document->objectId(data)) {
            };

            Node getNode(String fieldName) const {
                return Node{document_, document_->getNodeData(FieldId
                    {id_, fieldName})};
            };

            Node findNode(String fieldName) const {
                return Node{document_, document_->findNodeData(FieldId
                    {id_, fieldName)};};

                String getString(String fieldName) const {
                    return document_->string(document_->getNodeData(FieldId{id_, fieldName}));
                };

                String findString(String fieldName) const {
                    return document_->string(document_->findNodeData(FieldId{id_, fieldName}));
                };

                Number getNumber(String fieldName) const {
                    return document_->number(document_->getNodeData(FieldId{id_, fieldName}));
                };

                String findNumber(String fieldName) const {
                    return document_->number(document_->findNodeData(FieldId{id_, fieldName}));
                };

                Boolean getBoolean(String fieldName) const {
                    return document_->boolean(document_->getNodeData(FieldId{id_, fieldName}));
                };

                bool getNull(String fieldName) const {
                    return document_->null(document_->getNodeData(FieldId{id_, fieldName}));
                };

                bool findNull(String fieldName) const {
                    return document_->null(document_->findNodeData(FieldId{id_, fieldName}));
                };

                Object getObject(String fieldName) const {
                    return Object(document_, document_->getNodeData(FieldId{id_, fieldName}));
                };

                Object findObject(String fieldName) const {
                    return Object(document_, document_->findNodeData(FieldId{id_, fieldName}));
                };

                Array getArray(String fieldName) const;

                Array findArray(String fieldName) const;

            };

            class Array : public NodeBase {
            private:
                std::list<NodeData> *elements_;
            public:

                Array(const Document *document, NodeData data) : NodeBase(document, data), elements_(document->arrayElements(data)) {
                };

                class Iterator {
                private:
                    const Document *document_;
                    std::list<NodeData>::iterator iterator_;

                public:

                    Iterator(const Document *document, std::list<NodeData>::iterator iterator) : document_(document), iterator_(iterator) {
                    };

                    Node operator*() const {
                        return Node{document, *iterator_};
                    };

                    Node operator->() const {
                        return Node{document, *iterator_};
                    };

                    Iterator &operator++() {
                        ++iterator_;
                        return *this;
                    };

                    Iterator operator++(int) {
                        return Iterator{iterator_++};
                    };

                    bool operator==(const Iterator &i) const {
                        return iterator_ == i.iterator_;
                    };

                    bool operato != (const Iterator &i) const {
                        return iterator_ != i.iterator_;
                    };
                };

                using iterator = Iterator;

                Iterator begin() const {
                    return Iterator{document_, elements_->begin()};
                };

                Iterator end() const {
                    return Iterator{document_, elements_->end()};
                };

                bool empty() const {
                    return elements_->empty();
                };

                int size() const {
                    return elements_->size();
                };

            };

        };

        BufferedDocument() : fields_(), empty_(StoragePolicy::emptyNodeData()){};
        
        Node getRootNode() const{
            return Node{this, getNodeData()
        };
        
    };
}

#endif
