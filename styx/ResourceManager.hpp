#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

using Textures{
    enum ID { Player, Eyeball};
}

template<typename Resource, typename Identifier>
class ResourceHolder{
    public:
        void                load(Identifier id, const std::string& filename);
        void                load(Identifier id, const std::string& filename, const Parameter& secondParam);
        Resource&        get(Indentifier id);
        const Resource&  get(Identifier id) const;
    private:
        std::map<Identifier,std::unique_ptr<Resource>> mResourceMap;

};



void ResourceHolder<Resource,Identifier>::load(Identifier id, const std::string& filename){
    std::unique_ptr<Resource> Resource(new Resource());
    if (!resource->loadFromFile(filename)){
        throw std::runtime_error("ResourceHolder::load - Failed to load "+ filename);

    auto inserted = mResourceMap.insert(std::make_pair(id,std::move(resource)));
    //for debugging (in debug mode)
    //assert(inserted.second);
}

//function overload
template<typename Resource, typename Identifier>
template<typename Parameter>
void ResourceHolder<Resource,Identifier>::load(Identifier id, const std::string& filename,const Parameter& secondParam){
    std::unique_ptr<Resource> Resource(new Resource());
    if (!resource->loadFromFile(filename,secondParam)){
        throw std::runtime_error("ResourceHolder::load - Failed to load "+ filename);

    auto inserted = mResourceMap.insert(std::make_pair(id,std::move(resource)));
    //for debugging (in debug mode)
    //assert(inserted.second);
}


template<typename Resource, typename Identifier>
Resource& ResourceHolder<Resource,Identifier>::get(Identifier id){
    auto found = mResourceMap.find(id);
    //for debugging (in debug mode)
    //assert(found!=mResourceMap.end());
    return *found->second;
}



#endif // RESOURCEMANAGER_HPP_INCLUDED
