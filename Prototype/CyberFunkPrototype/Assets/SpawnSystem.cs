using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnSystem : MonoBehaviour
{
    [System.Serializable]
    public class Pool
    {
        public string m_tag;
        public GameObject m_prefab;
        public int size;
    }

    
    public List<Pool> m_objectsToSpawn;
    public Dictionary<string, Queue<GameObject>> m_pools;
    public float m_mapWidth;
    public float m_mapHeight;

    // Start is called before the first frame update
    void Start()
    {
        m_pools = new Dictionary<string, Queue<GameObject>>();

        foreach (Pool pool in m_objectsToSpawn)
        {
            Queue<GameObject> objectPool = new Queue<GameObject>();

            for (int i = 0; i < pool.size; i++)
            {
                GameObject obj = Instantiate(pool.m_prefab);
                obj.SetActive(false);
                objectPool.Enqueue(obj);
            }

            m_pools.Add(pool.m_tag, objectPool);


        }
    }

    // Update is called once per frame
    void Update()
    {
        SpawnObjectFromPool("Enemy");
        SpawnObjectFromPool("Beacon");
    }

    //Function to Spawn Enemy
    public void SpawnObjectFromPool (string tag)
    {
        if (!m_pools.ContainsKey(tag))
        {
            Debug.LogWarning(tag + " Pool does not exist");
        }
        else
        {

            //Get object from pool
            GameObject objectToSpawn = m_pools[tag].Dequeue();

            if (objectToSpawn.activeInHierarchy == false)
            {
                //Random Position in map
                Vector3 position;
                float posX, posY;

                //Random X and Y position
                posX = Random.Range(-(m_mapWidth / 2), (m_mapWidth / 2));
                posY = Random.Range(0, m_mapHeight);
                position = new Vector3(posX, posY);

                //Set object position and set it active
                objectToSpawn.transform.position = position;
                objectToSpawn.transform.rotation = transform.rotation;
                objectToSpawn.SetActive(true);
            }

            m_pools[tag].Enqueue(objectToSpawn);
        }


    }

}
