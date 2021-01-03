#include "BPTree.h"
#include "DiskManager.h"

BPTree::BPTree(Meta* meta, disk_pointer root)
{
    this->meta = meta;
    this->root = root;

} // End constructor

void BPTree::BPLeaf::print()
{

    if ( parent == NULL_DISK_POINTER )
        printf("p = dNULL ");
    else
        printf("p = %llu ", parent);

    printf(" l = %d ", isLeaf);
    printf(" f = %d ", filling);

    for (int i = 0; i < BP_POINTERS - 1; i++)
    {

        if ( diskPointers[i] == NULL_DISK_POINTER )
            printf("| dNULL |");
        else
            printf("| %llu |", diskPointers[i]);

        if ( keys[i] == NULL )
            printf(" pNULL ");
        else
        {
            printf(" ");
            for (int j = 0; j < meta->getPrimaryKeyByteSize(); j++ )
                printf("%d ", keys[i][j]);
        }

    } // End for

    if ( diskPointers[BP_POINTERS - 1] == NULL_DISK_POINTER )
        printf("| dNULL |\n");
    else
        printf("| %llu |\n", diskPointers[BP_POINTERS - 1]);    

} // End leaf

BPTree::BPLeaf::BPLeaf(Meta* meta)
{
    this->meta = meta;
    this->filling = 0;
    this->diskPointers = (disk_pointer*) malloc( sizeof(disk_pointer) * BP_POINTERS );
    this->keys = (char**) malloc( sizeof(char*) * (BP_POINTERS - 1) );

    for (int i = 0; i < BP_POINTERS; i++)
    {
        diskPointers[i] = NULL_DISK_POINTER;
    } // End for

    for (int i = 0; i < BP_POINTERS - 1; i++)
    {
        keys[i] = NULL;
    } // End for

    this->parent = NULL_DISK_POINTER;
    this->isLeaf = true;

} // End constructor

BPTree::BPLeaf::BPLeaf(Meta* meta, unsigned short filling, disk_pointer* diskPointers, char** keys, disk_pointer parent, bool isLeaf, disk_pointer diskLocation )
{
    this->meta = meta;
    this->filling = filling;
    this->diskPointers = diskPointers;
    this->keys = keys;
    this->parent = parent;
    this->isLeaf = isLeaf;
    this->diskLocation = diskLocation;

} // End constructor


/*
The method allows to register a new key and a disk pointer into the leaf
Input:  pointer and key with the values to add into the leaf 
Output: void
Warning: The method assumes that there's enough space in the leaf to carry out
such insertion
*/
void BPTree::BPLeaf::insert( disk_pointer pointer, char* key )
{

    unsigned short i;

    // Look for an appropiate position to insert
    for ( i = 0; i < BP_POINTERS -1 ; i ++ )
    {
        // Note: Using shortcut evaluation!
        // First check if the key at i-th position is NULL
        // If so, the position was found
        // Otherwise evaluate left-hand 
        // i.e. if the key to insert is shorter than the one at i-th position
        // if so, also the inserting position was found
        if ( ( keys[i] == NULL ) || ( Comparator::compareKeys(key, keys[i], meta) < 0) )
            break;
        
    } // End for

    // With the position already found:

    if ( i < filling )
    {
        // Right shift to open space for the new key
        shiftRight(i);

        if ( Comparator::compareKeys( key, keys[i], meta ) >= 0 || isLeaf )
            diskPointers[i] = NULL_DISK_POINTER;


    } // End if

    // Insert at freed position
    keys[i] = key;
    
    if ( diskPointers[i] == NULL_DISK_POINTER )
        diskPointers[i] = pointer;
    else
        diskPointers[i+1] = pointer;

    // Increase filling counter
    filling ++;

} // End insert


BPTree::BPLeaf* BPTree::searchLeaf( char* key )
{

    // Variables
    BPTree::BPLeaf* auxiliar;
    DiskManager* manager;
    unsigned short i;

    // Start at root
    auxiliar = manager->readBPLeafAt( root, meta );

    // Go downwards until a leaf is reached
    while ( ! auxiliar->isLeaf )
    {

        for ( i = 0; i < auxiliar->filling; i++ )
        {
            if ( Comparator::compareKeys( key, auxiliar->keys[i], meta ) < 0 )
                break;
                 
        } // End for

        // Move downwards
        auxiliar = manager->readBPLeafAt( auxiliar->diskPointers[i] , meta);
        
    } // End while

    return auxiliar;

} // End search

/*
The method allows to shift all keys and pointers to the right,
so that new keys on the current leaf can be added while maintaining
the order
Input:  position indicationg the position to free by right-shifting
Output: void
Warning: The method assumes that there's enough space in the leaf to carry out
such shifting
*/
void BPTree::BPLeaf::shiftRight( unsigned short position )
{

    // Check if the right pointer is occupied if so shift it previously
    if ( diskPointers[filling] != NULL_DISK_POINTER )
    {
        diskPointers[filling + 1] = diskPointers[filling];
    } // End if

    // Move all keys and disk pointers one position to right
    // freeing the cell indicated by position
    for ( int i = filling; i > position; i -- )
    {
        keys[i] = keys[i-1];
        diskPointers[i] = diskPointers[i-1];
    } // End for

} // End shiftRight

/*
The method allows to split a leaf by half 
Input:  position indicationg the position to free by right-shifting
Output: void
Warning: The method assumes that there's enough space in the leaf to carry out
such shifting
*/
BPTree::BPLeaf* BPTree::BPLeaf::split()
{
    unsigned short cutPoint;
    unsigned short newLeafOffset;
    int i;
    BPTree::BPLeaf* newLeaf;

    // Calculate half of the leaf: BP_POINTERS is already L + 1 keys
    cutPoint = BP_POINTERS / 2;
    newLeaf = new BPTree::BPLeaf ( meta );
    newLeaf->isLeaf = isLeaf;

    // Offset of writing in the new leaf
    newLeafOffset = 0;

    // Copy in order so that insertion call is avoided (slower)
    for ( i = cutPoint; i < BP_POINTERS -1; i ++ )
    {
        // Copy disk pointers
        newLeaf->diskPointers[newLeafOffset] = diskPointers[i];

        if (isLeaf || i != cutPoint)
            diskPointers[i] = NULL_DISK_POINTER;

        // Copy keys only if possible 
        if (i < BP_POINTERS -1 ){
            newLeaf->keys[newLeafOffset] = keys[i];
            keys[i] = NULL;

            // Decrease filling counters
            newLeaf->filling++;
            filling--;

        } // End if

        // Increase the offset in the new leaf
        newLeafOffset++;

    } // End for

    if (!isLeaf){
        newLeaf->diskPointers[newLeafOffset] = diskPointers[i];
        diskPointers[i] = NULL_DISK_POINTER;
    } // End if

    return newLeaf;

} // End shiftRight

void BPTree::BPLeaf::shiftLeft()
{
    
    for ( int i = 0; i < filling; i++ )
    {
        keys[i] = keys[i+1];
        diskPointers[i] = diskPointers[i+1];
    } // End for
    
    diskPointers[filling] = NULL_DISK_POINTER;
    filling--;

} // End shiftLeft


unsigned int BPTree::BPLeaf::getSerialFormSize()
{
    return  sizeof(disk_pointer) * BP_POINTERS + 
            meta->getPrimaryKeyByteSize() * (BP_POINTERS - 1) +
            sizeof (unsigned short) + sizeof(disk_pointer) + sizeof(isLeaf);

} // End getSerialFormSize

char* BPTree::BPLeaf::serialize()
{
    // Variables
    char* serializedForm;
    char* offset;

    // Require space in the heap
    serializedForm = (char*) malloc( getSerialFormSize() );
    offset = serializedForm;

    // Copy parent
    memcpy(offset, &parent, sizeof(disk_pointer));
    offset = (char*) (offset + sizeof(disk_pointer));

    // Copy filling counter
    memcpy(offset, &filling, sizeof(unsigned short));
    offset = (char*) (offset + sizeof(unsigned short));

    // Copy leaf indicator
    memcpy(offset, &isLeaf, sizeof(bool));
    offset = (char*) (offset + sizeof(bool));

    // Copy pointers
    memcpy(offset, diskPointers, sizeof(disk_pointer) * BP_POINTERS);
    offset = (char*) (offset + (sizeof(disk_pointer) * BP_POINTERS));

    // Copy keys 
    for (int i = 0; i < filling; i++)
    {
        memcpy(offset, keys[i], meta->getPrimaryKeyByteSize());
        offset = (char*) (offset + meta->getPrimaryKeyByteSize());
    } // End for 


    return serializedForm;

} // End serialize

unsigned int BPTree::getSerialFormSize()
{
    return sizeof(disk_pointer);

} // End getSerialFormSize

char* BPTree::serialize()
{
    // Variables
    char* serializedForm;
    char* offset;

    // Require space in the heap
    serializedForm = (char*) malloc( getSerialFormSize() );

    // Copy root pointer
    memcpy(serializedForm, &root, sizeof(disk_pointer));
    offset = (char*) (offset + sizeof(disk_pointer));
    
    return serializedForm;

} // End serialize

void BPTree::insert( disk_pointer location, char* primary )
{
    BPLeaf*         auxiliar;
    BPLeaf*         splittedLeaf;
    BPLeaf*         newRoot;
    BPLeaf*         updatingLeaf;
    DiskManager*    manager;
    bool            hasRemainingInsertions;

    // Check if there is no root yet
    if ( root == NULL_DISK_POINTER )
    {
        // Create root leaf
        auxiliar = new BPTree::BPLeaf( meta );
        manager->insertBPLeaf(auxiliar);

        // Fix root pointer
        root = auxiliar->diskLocation;
        manager->storeBPTree(this);

    } // End if 

    else
    {
        auxiliar = searchLeaf( primary ); 
    } // End else


    hasRemainingInsertions = true;

    while (hasRemainingInsertions) {

        // Insertion cases:
        // 1. The key fits into current leaf
        if ( auxiliar->filling < BP_POINTERS -1 )
        {   
            auxiliar->insert( location, primary );
            manager->updateBPLeaf(auxiliar);
            hasRemainingInsertions = false;
        } // End if 

        // 2. Split and insert
        else
        {
            // Split prior to insertion
            splittedLeaf = auxiliar->split();
            manager->insertBPLeaf(splittedLeaf);

            // Connect leaves if they're
            if (auxiliar->isLeaf){
                splittedLeaf->diskPointers[BP_POINTERS - 1] = auxiliar->diskPointers[BP_POINTERS - 1];
                auxiliar->diskPointers[BP_POINTERS - 1] = splittedLeaf->diskLocation;
            } // End if

            // Check where to insert new key and carry out such process
            if ( Comparator::compareKeys(primary, splittedLeaf->keys[0], meta ) < 0 )
                auxiliar->insert( location, primary );
            else
                splittedLeaf->insert( location, primary );

            primary = splittedLeaf->keys[0];
            location = splittedLeaf->diskLocation;

            // Transfer to splitted node last entry to balance the tree
            if (!splittedLeaf->isLeaf)
                splittedLeaf->shiftLeft();

            // Update parents upon splittage of a non-leaf
            if (!splittedLeaf->isLeaf)
            {
                for (int i = 0; i < BP_POINTERS; i ++ )
                {
                    if (splittedLeaf->diskPointers[i] == NULL_DISK_POINTER)
                        break;
                    
                    updatingLeaf = manager->readBPLeafAt( splittedLeaf->diskPointers[i], meta );
                    updatingLeaf->parent = splittedLeaf->diskLocation;
                    manager->updateBPLeaf(updatingLeaf);

                } // End for

            } // End if 

            // Check if parent is null. If so create a new root
            if ( auxiliar->parent == NULL_DISK_POINTER )
            {
            
                // Create new leaf and point it to the both splitted leaves
                newRoot = new BPTree::BPLeaf(meta);
                newRoot->insert( auxiliar->diskLocation, primary );
                newRoot->diskPointers[1] = location;
                newRoot->isLeaf = false;
                
                // Record new leaf onto the permanent storage
                manager->insertBPLeaf(newRoot);
                root = newRoot->diskLocation;
                manager->storeBPTree(this);

                // Set parents and record
                auxiliar->parent = newRoot->diskLocation;
                splittedLeaf->parent = newRoot->diskLocation;
                manager->updateBPLeaf(auxiliar);
                manager->updateBPLeaf(splittedLeaf);

                hasRemainingInsertions = false;

            } // End if

            else
            {

                splittedLeaf->parent = auxiliar->parent;
                manager->updateBPLeaf(auxiliar);
                manager->updateBPLeaf(splittedLeaf);

                // Move to parent
                auxiliar = manager->readBPLeafAt( auxiliar->parent, meta );

            } // End else

        } // End else

    } // End while

} // End insert