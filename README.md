# Tree-ANN-indices-KD-Tree-and-RP-Tree
• Developed a class of ANN indices using tree-based structures (KDTree and RPTree) to efficiently search for k-nearest neighbors in a high-dimensional vector dataset.

• Implemented singleton classes for KDTreeIndex and RPTreeIndex, ensuring a single instance manages all operations for the respective index type.

• Designed and implemented AddData and RemoveData methods to dynamically manage the vector dataset within the indices.

• Created the MakeTree function to build hierarchical tree structures for both KDTree and RPTree, utilizing the ChooseRule function for optimal vector space partitioning.

• Developed an efficient Search algorithm that navigates through the tree structure to identify the k-nearest neighbors of a given test vector, leveraging backtracking and boundary distance calculations.
