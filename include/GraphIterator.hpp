//
// Created by Mateusz Miliutin on 12/05/2026.
//

#ifndef ALGOSY2GRAFY_GRAPHITERATOR_HPP
#define ALGOSY2GRAFY_GRAPHITERATOR_HPP

// TODO consider removing base class as a whole if it is not considered good practice to have polymorphic iterators
template <typename T>
class GraphIterator {
public:
    GraphIterator() = default;
    virtual ~GraphIterator() = default;
    GraphIterator(const GraphIterator& other) = default;
    GraphIterator& operator=(const GraphIterator& other) = default;
    // Sa problemy w dziedziczeniu tych funkcji.
    virtual GraphIterator& operator++() = 0;

    // !!! Nie działa bo jest niezgodność zwracanego typu w klasie pochodnej. STL w ogóle nie używa wirtualnych iteratowór
    // źr.: https://stackoverflow.com/questions/3958976/why-is-there-no-iterable-interface-in-the-stl
    // virtual GraphIterator operator++(int) = 0;
    // virtual bool operator==(const GraphIterator& other) = 0;
    // virtual bool operator!=(const GraphIterator& other) = 0;
    virtual T operator*() const = 0; // działa
};


#endif //ALGOSY2GRAFY_GRAPHITERATOR_HPP