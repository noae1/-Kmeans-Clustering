import sys
import math
import os
script_dir = os.path.dirname(__file__)

__epsilon__=0.001

def calcCentroids():
    # k, iter, file_path = 3, 100, os.path.join(script_dir,"input_1.txt")
    # file_path = os.path.join(script_dir,file_path)

    if len(sys.argv)!=4 and len(sys.argv)!= 3:
        print("An Error Has Occurred")
        exit()
    if len(sys.argv) ==4:
        k, iter, file_path = sys.argv[1], sys.argv[2], sys.argv[3] #todo: check if int
    else:
        k, file_path = sys.argv[1], sys.argv[2]
        iter='200'

    def d(dataItem,centroid):
        return math.sqrt(sum([(dataItem[i]-centroid[i])**2 for i in range(len(dataItem))]))


    def prepData(data):
        res = []
        for line in data.readlines():
            innerlist=[]
            for val in line.split(","):
                innerlist.append(float(val))
            res.append(innerlist)
        return res

    with open(file_path, 'r') as rawData:
        data = prepData(rawData)
    if not (iter).isnumeric():
        print("Invalid maximum iteration!")
        sys.exit()
    if not (k).isnumeric():
        print("Invalid number of clusters!")
        sys.exit()
    k, iter = int(k), int(iter)
    if (k)%1!=0 or k<=1 or k >= len(data):
        print("Invalid number of clusters!")
        sys.exit()
    if iter%1!=0 or iter<=1 or iter >= 1000:
        print("Invalid maximum iteration!")
        sys.exit()
    iteration_number=1
    centroids= [item for item in data[:k]]
    asignments = {}
    maxCentroidDelta=__epsilon__+1
    while(iteration_number < iter and maxCentroidDelta>=__epsilon__):
        maxCentroidDelta=0
        asignments = {}
        for i in range(len(data)):
            xi=data[i]
            minD =(0, d(xi,centroids[0]))
            for j in range(1, k):
                currD= d(xi,centroids[j])
                if minD[1]>currD:
                    minD =(j,currD)
            if minD[0] not in asignments:
                asignments[minD[0]] = []
            asignments[minD[0]].append(i)
        for j in range(len(centroids)):
            if j in asignments:

                assignedData= asignments[j]
                newJCentroid = [0 for item in data[0]]
                for l in assignedData:
                    newJCentroid=[a+b for a,b in zip(newJCentroid,data[l])]
                if(len(assignedData)>0):
                    newJCentroid = [item*(1/(len(assignedData))) for item in newJCentroid]

                centroidsDelta = d(newJCentroid,centroids[j])
                centroids[j] = newJCentroid
                if centroidsDelta>maxCentroidDelta:
                    maxCentroidDelta= centroidsDelta
        iteration_number+=1


    for centroid in centroids:
        print(','.join('{:.4f}'.format(v) for v in centroid))


calcCentroids()