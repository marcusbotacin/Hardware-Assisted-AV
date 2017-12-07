# Hardware-Assisted-AV
Additional material for the Hardware Assisted AV paper

## Authors
  
Msc. [Marcus Botacin](www.inf.ufpr.br/mfbotacin), under supervision of [Prof. Dr. Marco Zanata](http://web.inf.ufpr.br/mazalves/) and [Prof.   Dr. André Grégio](https://sites.google.com/site/argregio/) -- [Department of Informatics](http://web.inf.ufpr.br/dinf/) - [Federal University  of Paraná](http://www.ufpr.br/portalufpr/) -- and [Profª Drª. Daniela Oliveira](http://www.daniela.ece.ufl.edu/Home.html).
    
## Goal
     
Discuss the use of branch history as signature for malware detection.
        
## Repository Organization
          
* **Branch.Framework**: A two-level architecture solution which generates branch history patterns and match them against known signatures, trigerring AV scans.
** **Branch.Framework/Hardware**: The hardware component; A PIN-based tool responsible for branch pattern generation and signature matching.
** **Branch.Framework/Software**: The software component; An ordinary AV responsible for False Positive elimination/disambiguation.

* **Signature.Generation**: Approaches for signature generation from branch data.
* **Signature.Generation/Whitelist**: The usual way of generating signatures.
* **Signature.Generation/Neural.Network**: An approach to reduce the signature search space.

## Paper

**To Be Published**
